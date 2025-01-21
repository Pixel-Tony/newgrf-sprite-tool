#include "Mytec/model/image_manager.hpp"
#include "Mytec/tools/pen.hpp"
#include "Mytec/ui/style.hpp"
#include "Mytec/ui/misc.hpp"
#include "Mytec/util/file_dialog.hpp"

#include <functional>
#include <numeric>
#include <vector>
#include <ranges>

#include "Mytec/ui/button.hpp"
#include "Mytec/ui/dialogs.hpp"
#include "Mytec/util/str.hpp"


namespace
{
namespace fs = std::filesystem;
using namespace mytec;
using tool_factory_t = std::function<std::unique_ptr<tools::tool_t>()>;

constexpr auto image_tab_style = static_cast<ImGuiTabBarFlags_>(
    ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_NoTooltip
    | ImGuiTabBarFlags_NoTabListScrollingButtons | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll
);

inline constexpr auto w_icon = 24.f;
inline constexpr auto icon_size = v2f{1, 1} * w_icon;
inline constexpr v2f tool_bar_pad = {2, 2};
inline constexpr auto bar_w = w_icon + 2 * tool_bar_pad.y;

const std::vector<std::tuple<tools::tool_id, ui::button, tool_factory_t>> tool_icons{
    {
        tools::tool_id::view,
        ui::button{
            "BtnView",
            "../../assets/icons/view.png",
            "../../assets/icons/view-hover.png",
            "../../assets/icons/view-active.png"
        },
        std::make_unique<tools::view>
    },
    {
        tools::tool_id::pen,
        ui::button{
            "BtnPen",
            "../../assets/icons/pen.png",
            "../../assets/icons/pen-hover.png",
            "../../assets/icons/pen-active.png"
        },
        std::make_unique<tools::pen>
    }
};

const auto msg_file_already_open = "File is already open in the editor";
const auto msg_cannot_save_already_open = "Destination file is already open in the editor";
const auto msg_cannot_save = "Could not save the file";
const auto msg_could_not_open = "Could not open the file";

template <class T> requires (std::is_base_of_v<ui::popup_modal, T>)
void update_modal(std::unique_ptr<T> &ptr)
{
    if (ptr && !ptr->update())
        ptr.reset();
}
}

namespace mytec::model
{
image_manager::image_manager(const sf::WindowHandle &handle): file_dialog(handle), tool(std::make_unique<tools::view>())
{
}

std::vector<image>::const_iterator image_manager::find_image_open(const fs::path &filepath) const
{
    return std::ranges::find_if(images, [&](const image &other)
    {
        const fs::path &rhs = other.get_path();
        return !rhs.empty() && 0 == filepath.compare(rhs);
    });
}

void image_manager::show_error(const char *message)
{
    message_box = std::make_unique<ui::message_box>("Error", message);
}

bool image_manager::is_image_open(const fs::path &path) const { return find_image_open(path) != images.cend(); }

void image_manager::open_file()
{
    const fs::path filepath = file_dialog.try_open();
    if (filepath.empty())
        return;

    if (is_image_open(filepath))
        return show_error(msg_file_already_open);

    std::optional result = image::open(filepath);
    if (!result)
    {
        return show_error(msg_could_not_open);
    }
    images.emplace_back(std::move(result.value()));
}

bool image_manager::try_resave(image &target)
{
    const auto path = file_dialog.try_save(&target.get_name());
    if (path.empty())
    {
        return false;
    }
    if (const auto path_owner = find_image_open(path);
        path_owner != images.cend() && (target.exists() || *path_owner != target))
    {
        show_error(msg_cannot_save_already_open);
        return false;
    }
    if (!target.try_save(path))
    {
        show_error(msg_cannot_save);
        return false;
    }
    return true;
}

void image_manager::handle_events()
{
    if (tool->engaged())
        return;

    if (ImGui::IsKeyPressed(ImGuiKey_V))
    {
        tool = std::make_unique<tools::view>();
    }
    else if (ImGui::IsKeyPressed(ImGuiKey_B))
    {
        tool = std::make_unique<tools::pen>();
    }
}

bool image_manager::try_close(bool &want_close)
{
    if (message_box || dialog_new_image || dialog_save_closing_image)
    {
        return false;
    }
    while (images.size() > 0)
    {
        if (image &last = images.back(); last.is_valid() && last.dirty())
            break;
        images.pop_back();
    }
    if (images.empty())
    {
        return true;
    }
    image &cur_image = images.back();

    // images stay constant until at least after dialog is closed
    dialog_save_closing_image = std::make_unique<ui::save_closing_image>(
        cur_image,
        [&](const std::pair<bool, bool> status)
        {
            const auto &[saved, closedWithoutSave] = status;
            if (closedWithoutSave)
                return cur_image.close();

            if (!saved)
            {
                want_close = false;
                return;
            }
            if (try_resave(cur_image))
                cur_image.close();
        }
    );
    return false;
}

void image_manager::render_menu_bar_entry(const size_t i_cur_image)
{
    if (!ImGui::BeginMainMenuBar())
        return;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
    ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetStyle().Colors[ImGuiCol_MenuBarBg]);
    if (ImGui::BeginMenu("File"))
    {
        bool valid = false;
        if (ImGui::MenuItem("New file..."))
        {
            valid = false;
            dialog_new_image = std::make_unique<ui::create_new_image>([&](image &&created) -> void
            {
                images.emplace_back(std::move(created));
            });
        }
        if (ImGui::MenuItem("Open..."))
        {
            valid = false;
            open_file();
        }
        if (!valid || i_cur_image == i_no_cur_image)
        {
            ImGui::BeginDisabled();
            ImGui::MenuItem("Save");
            ImGui::MenuItem("Save as...");
            ImGui::EndDisabled();
        }
        else
        {
            auto &cur_image = images[i_cur_image];
            if (ImGui::MenuItem("Save", nullptr, nullptr, cur_image.exists() && cur_image.dirty()))
            {
                if (!cur_image.try_save())
                {
                    show_error(msg_cannot_save);
                }
            }
            if (ImGui::MenuItem("Save as..."))
            {
                try_resave(cur_image);
            }
        }
        ImGui::EndMenu();
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::EndMainMenuBar();
}

void image_manager::update()
{
    std::erase_if(images, [](const image &image) { return !image.is_valid(); });

    handle_events();

    size_t i_cur_image = i_no_cur_image;
    const float h_status_bar = ImGui::GetFontSize() + 2 * 2;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});
    {
        const auto vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(vp->WorkPos);
        ImGui::SetNextWindowSize(vp->WorkSize);
    }
    if (ImGui::Begin("##Fullscreen", nullptr, ui::style::win_clear))
    {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);
        if (ImGui::BeginChild("##Body", {-bar_w, -h_status_bar}))
        {
            if (ImGui::BeginTabBar("##Canvas", image_tab_style))
            {
                for (auto it = images.begin(), end = images.end(); it < end; ++it)
                {
                    image &cur_image = *it;
                    auto [isShown, isOpen] = cur_image.update(tool, editor_);
                    if (isShown)
                    {
                        i_cur_image = std::distance(images.begin(), it);
                    }
                    if (!isOpen && cur_image.is_valid())
                    {
                        dialog_save_closing_image = std::make_unique<ui::save_closing_image>(
                            cur_image,
                            [&](const std::pair<bool, bool> status) -> void
                            {
                                const auto &[saved, closed] = status;
                                if (closed)
                                    return cur_image.close();

                                if (!saved)
                                    return;

                                if (try_resave(cur_image))
                                    cur_image.close();
                            }
                        );
                    }
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();

        ImGui::SameLine();
        ui::move_cursor_screen_pos({0, 1});
        if (ImGui::BeginChild("##SideBar", {bar_w, -(h_status_bar + 1)}, 0)) // 1px pad
        {
            ImGui::Dummy(tool_bar_pad * v2f{0, 1}); // y-pad
            const auto id = tool->id();
            for (const auto &[id_other, button, factory]: tool_icons)
            {
                const auto is_active = id == id_other;
                ui::move_cursor_screen_pos(tool_bar_pad * v2f{1, 0});
                if (button(icon_size, is_active))
                {
                    tool = factory();
                }
            }
        }
        ImGui::EndChild();

        if (ImGui::BeginChild("##StatusBar", {0, h_status_bar}))
        {
            if (i_cur_image != i_no_cur_image)
            {
                const auto &img = images[i_cur_image];
                const auto zoom = static_cast<int32_t>(100 * img.get_zoom());
                ui::move_cursor_screen_pos({15, 2}); // pad
                ImGui::TextUnformatted(std::format(
                    "{}{} @  {}%",
                    util::ellipsis(img.get_name().string(), 40),
                    img.dirty() ? '*' : ' ',
                    zoom
                ).c_str());
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
    ImGui::PopStyleVar(2);

    update_modal(dialog_new_image);
    update_modal(dialog_save_closing_image);
    update_modal(message_box);

    render_menu_bar_entry(i_cur_image);
}
}
