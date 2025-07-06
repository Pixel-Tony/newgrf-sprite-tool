#include "mytec/ui/components/dialogs.hpp"

#include <algorithm>
#include <imgui.h>
#include <set>
#include <string_view>

namespace
{
void make_label(const char* _name, const float _width)
{
    ImGui::TableNextColumn();
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(_name);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(_width);
}
} // namespace

namespace mytec
{
create_new_image::create_new_image(callback_t&& _bound)
    : promise_popup("Create new image", std::forward<callback_t>(_bound), false, true)
{
    file_name_.assign("Unnamed.png");
    file_name_.resize(128);
}

const char* create_new_image::validate() const noexcept
{
    static std::set<char> invalid = {'<', '>', ':', '"', '/', '\\', '|', '?', '*'};
    static std::set<std::string_view> invalid_paths = {"..", "."};

    if (size_ != clamp(size_, {1, 1}, {5000, 5000}))
        return "Image dimensions should be in bounds from 1 to 5000.";

    if (std::ranges::any_of(file_name_, [&](auto _c) { return invalid.contains(_c); }))
        return "Name contains forbidden characters.";

    if (invalid_paths.contains(std::string_view{file_name_}.substr(0, file_name_.find('\0'))))
        return "Name is invalid";

    return nullptr;
}

void create_new_image::body()
{
    if (ImGui::BeginTable("##NewFileProperties", 2))
    {
        make_label("New file name", 180);
        ImGui::InputText("##NewFileName", file_name_.data(), file_name_.size());

        make_label("Width", 180);
        ImGui::InputInt("##Width", &size_.x, 0);

        make_label("Height", 180);
        ImGui::InputInt("##Height", &size_.y, 0);

        make_label("Palette", 180);
        ImGui::Combo("##Palette", reinterpret_cast<int32_t*>(&palette_), "Windows\0DOS\0");
        ImGui::EndTable();
    }
    ImGui::Dummy({0, 4});
    const auto pos = ImGui::GetCursorScreenPos();
    static constexpr vf2 btn_size = {100, 0};
    if (const auto* const err = validate())
    {
        ImGui::BeginDisabled();
        ImGui::Button("Create", btn_size);
        ImGui::SetItemTooltip("%s", err);
        ImGui::EndDisabled();
    }
    else if (ImGui::Button("Create", btn_size) || ImGui::IsKeyPressed(ImGuiKey_Enter))
    {
        file_name_.resize(file_name_.find('\0'));
        resolve(image{std::move(file_name_), vu2{size_}, palette_});
    }
    ImGui::SetCursorScreenPos(pos + ImGui::GetContentRegionAvail() * vf2{1, 0} - btn_size); // Justify between
    if (ImGui::Button("Cancel", btn_size) || ImGui::IsKeyPressed(ImGuiKey_Escape))
        close();
}
} // namespace mytec
