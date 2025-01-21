#include "Mytec/ui/dialogs.hpp"
#include "Mytec/ui/misc.hpp"
#include "Mytec/util/str.hpp"

#include <utility>


namespace
{
void label_plus_width_spec(const char *name, const float width = 180.f)
{
    ImGui::TableNextColumn();
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(name);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(width);
}
}

namespace mytec::ui
{
create_new_image::create_new_image(callback_t &&bound)
    : promise_popup("Create new image", std::forward<callback_t>(bound))
{
    file_name.assign("Unnamed.png");
    file_name.resize(128);
}

void create_new_image::body()
{
    if (ImGui::BeginTable("##NewFileProperties", 2))
    {
        label_plus_width_spec("New file name");
        ImGui::InputText("##NewFileName", file_name.data(), file_name.size());

        label_plus_width_spec("Width");
        ImGui::InputInt("##Width", &size.x, 0, 0, ImGuiInputTextFlags_CharsDecimal);

        label_plus_width_spec("Height");
        ImGui::InputInt("##Height", &size.y, 0, 0, ImGuiInputTextFlags_CharsDecimal);

        label_plus_width_spec("Palette");
        ImGui::Combo("##Palette", reinterpret_cast<int *>(&palette), "Full Color" "\0" "Windows" "\0" "DOS" "\0");
        ImGui::EndTable();
    }
    const auto pos = ImGui::GetCursorScreenPos();
    // Justify between
    constexpr v2f btn_size = {100, 0};
    if (ImGui::Button("Create", btn_size))
    {
        file_name.resize(file_name.find('\0'));
        resolve(model::image{std::move(file_name), v2u{size}, palette});
    }
    ImGui::SetCursorScreenPos(pos + ImGui::GetContentRegionAvail() * v2f{1, 0} - btn_size);
    if (ImGui::Button("Cancel", btn_size))
    {
        close();
    }
}


save_closing_image::save_closing_image(const model::image &target, callback_t &&bound)
    : promise_popup("Unsaved file", std::forward<callback_t>(bound), false),
      message(std::format("Image \"{}\" has unsaved changes",
                          util::ellipsis(target.get_name().stem().string(), 25)))
{
}

void save_closing_image::body()
{
    ImGui::TextUnformatted(message.c_str());
    ImGui::Dummy({0, 10});
    constexpr v2f btn_size = {80, 0};
    if (ImGui::Button("Save", btn_size))
        resolve({true, false});
    ImGui::SameLine();
    if (ImGui::Button("Close", btn_size))
        resolve({false, true});
    ImGui::SameLine();
    if (ImGui::Button("Cancel", btn_size))
        resolve({});
}
}
