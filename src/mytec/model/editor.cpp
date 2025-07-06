#include "mytec/model/editor.hpp"

namespace mytec
{
std::optional<editor::click_info> editor::get_click_info() const
{
    const auto& clicks = ImGui::GetIO().MouseClicked;
    if (clicks[0])
        return click_info{.button_ = ImGuiMouseButton_Left, .color_ = primary_};
    if (clicks[1])
        return click_info{.button_ = ImGuiMouseButton_Right, .color_ = secondary_};
    return std::nullopt;
}

void editor::swap_colors() { std::swap(primary_, secondary_); }
} // namespace mytec
