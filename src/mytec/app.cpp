#include "mytec/app.hpp"

#include "mytec/file_dialog.hpp"
#include "mytec/model/tools/fill.hpp"
#include "mytec/model/tools/pen.hpp"
#include "mytec/model/tools/view.hpp"
#include "mytec/ui/components/dialogs.hpp"
#include "mytec/ui/components/status_bar.hpp"
#include "mytec/ui/event.hpp"
#include "mytec/ui/misc.hpp"

#include <imgui-SFML.h>

namespace
{
const auto err_could_not_open = "Could not open the file";
const auto err_file_already_open = "File is already open in the editor";
const auto err_could_not_save = "Could not save the file";

const auto canvas_flags = ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_Reorderable
                          | ImGuiTabBarFlags_NoTooltip | ImGuiTabBarFlags_NoTabListScrollingButtons
                          | ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_FittingPolicyScroll;
} // namespace

namespace mytec
{
app::app(const char* _title, const vu2 _size, const vu2 _min_size, const sf::Image& _icon, style _style, uint32_t _fps)
    : window_(sf::VideoMode{_size}, _title),
      style_(_style),
      status_bar_(style_),
      palette_tab_(style_),
      menu_bar_(style_),
      side_bar_(style_, status_bar_),
      tool_(std::make_unique<view>()),
      cur_palette_(palette::dos)
{
    window_.setMinimumSize(_min_size);
    window_.setVerticalSyncEnabled(false);
    window_.setFramerateLimit(_fps);
    window_.setIcon(_icon);

    if (!ImGui::SFML::Init(window_, false))
        throw std::runtime_error{"Could not initialize ImGui context."};

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    style_.init_font(io);
}

app::~app() { ImGui::SFML::Shutdown(window_); }

int app::run()
{
    window_.resetGLStates();
    style_.set_active();

    // <https://github.com/SFML/imgui-sfml/issues/206#issuecomment-2239907726>
    ImGui::SFML::ProcessEvent(window_, sf::Event::FocusLost{});
    ImGui::SFML::ProcessEvent(window_, sf::Event::FocusGained{});
    sf::Clock dt;
    while (window_.isOpen())
    {
        event ev;
        while (const auto opt_ev = window_.pollEvent())
        {
            const auto& win_ev = opt_ev.value();
            ImGui::SFML::ProcessEvent(window_, win_ev);
            want_close_ |= win_ev.is<sf::Event::Closed>();
            ev.make_on(want_close_, event_type::quit);

            if (const auto* const resized = win_ev.getIf<sf::Event::Resized>())
            {
                const auto size = resized->size;
                window_.setView({vf2{size} / 2.f, vf2{size}});
                window_.setSize(size);
            }
        }
        ImGui::SFML::Update(window_, dt.restart());

        window_.clear(style_.background_);
        update(ev);
        ImGui::SFML::Render(window_);
        window_.display();
    }
    return EXIT_SUCCESS;
}

void app::update(event _event)
{
    render_fullscreen(_event);
    menu_bar_.update(_event, !is_user_engaged(), tool_);
    handle_shortcuts(_event);
    palette_tab_.update();
    handle_event(_event);
    modals_.update();
}

void app::handle_shortcuts(event& _event)
{
    _event.on_shortcut(ImGuiMod_Ctrl | ImGuiKey_N, event_type::create);
    _event.on_shortcut(ImGuiMod_Ctrl | ImGuiKey_O, event_type::open);
    if (_event.image_)
    {
        _event.on_shortcut(ImGuiMod_Ctrl | ImGuiKey_W, event_type::close_image);
        _event.on_shortcut(ImGuiMod_Ctrl | ImGuiKey_S, event_type::save);
        _event.on_shortcut(ImGuiMod_Ctrl | ImGuiKey_Z, event_type::undo);
        _event.on_shortcut(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_S, event_type::save_as);
        _event.on_shortcut(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_Z, event_type::redo);
    }
    _event.on_shortcut(ImGuiKey_X, event_type::swap_colors);
    _event.tool_on_shortcut(ImGuiKey_V, tool_id::view);
    _event.tool_on_shortcut(ImGuiKey_B, tool_id::pen);
    _event.tool_on_shortcut(ImGuiKey_F, tool_id::fill);
}

void app::handle_event(const event _event)
{
    if (is_user_engaged() && _event.type_ != event_type::quit)
        return;

    auto* const img = _event.image_;
    switch (_event.type_)
    {
    case event_type::toggle_palette_visibility:
        palette_tab_.is_open_ ^= true;
        return;
    case event_type::quit:
        modals_.on_exit();
        if (!modals_.empty() || tool_->is_engaged())
            return;
        images_.on_exit();
        if (images_.empty())
            window_.close();
        else
            try_save_closing_image(&images_.back());
        return;

    case event_type::create:
        modals_.push<create_new_image>([&](image&& _created) { images_.push_back(std::move(_created)); });
        return;
    case event_type::open:
    {
        auto path = file_dialog::open();
        if (!path)
            return;
        if (images_.find_if_path(*path) != images_.cend())
        {
            show_error(err_file_already_open);
            return;
        }
        auto result = image::open(std::move(*path));
        if (!result)
        {
            show_error(err_could_not_open);
            return;
        }
        images_.push_back(std::move(*result));
    }
        return;
    case event_type::save:
        if (!img->exists())
            try_save_image_as(*img);
        else if (!img->is_dirty())
            return;
        else if (!img->try_save())
            show_error(err_could_not_save);
        return;
    case event_type::save_as:
        try_save_image_as(*img);
        return;
    case event_type::close_image:
        if (!img->is_dirty())
            std::erase(images_, *img);
        else
            try_save_closing_image(img);
        return;
    case event_type::choose_tool:
        switch (_event.next_tool_)
        {
        case tool_id::view:
            tool_ = std::make_unique<view>();
            return;
        case tool_id::pen:
            tool_ = std::make_unique<pen>();
            return;
        case tool_id::fill:
            tool_ = std::make_unique<fill>();
            return;
        default:
            return;
        }
    case event_type::undo:
        if (img)
            img->try_undo();
        return;
    case event_type::redo:
        if (img)
            img->try_redo();
        return;
    case event_type::swap_colors:
        editor_.swap_colors();
        return;
    case event_type::none:
        return;
    }
}

void app::render_fullscreen(event& _event)
{
    auto* const vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    if (ImGui::Begin("##Fullscreen", nullptr,
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoInputs
                | ImGuiWindowFlags_NoDecoration))
    {
        render_canvas(_event);
        ImGui::SameLine();
        move_cursor_screen_pos({0, 1}); // border
        side_bar_.update(_event, editor_, tool_);
        status_bar_.update(_event);
    }
    ImGui::End();
}

void app::render_canvas(event& _event)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, style_.background_);
    if (ImGui::BeginChild("##Body", {-side_bar::width, -status_bar_.height_})
        && ImGui::BeginTabBar("##Canvas", canvas_flags))
    {
        for (auto& img : images_)
        {
            auto [is_shown, is_open] = img.update(tool_, editor_);
            if (!is_shown)
                continue;
            _event.image_ = &img;
            _event.make_on(!is_open, event_type::close_image);
        }
        ImGui::EndTabBar();
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void app::show_error(const char* _message) { modals_.push<message_box>("Error", _message); }

bool app::is_user_engaged() const noexcept { return tool_->is_engaged() || (modals_.size() > 0); }

void app::try_save_closing_image(image* const _img)
{
    modals_.push<save_closing_image>(*_img,
        [&](const bool _cancelled, const bool _saved) -> void
        {
            if (_cancelled)
                want_close_ = false;
            else if (!_saved || try_save_image_as(*_img))
                std::erase(images_, *_img);
        });
}

bool app::try_save_image_as(image& _target)
{
    auto path = file_dialog::save(_target.get_name(false));
    if (!path)
        return false;

    if (const auto path_owner = images_.find_if_path(*path);
        path_owner != images_.cend() && _target.exists() && *path_owner != _target)
    {
        show_error(err_file_already_open);
        return false;
    }
    return false;
    if (!_target.try_save(*path))
    {
        show_error(err_could_not_save);
        return false;
    }
    return true;
}

} // namespace mytec
