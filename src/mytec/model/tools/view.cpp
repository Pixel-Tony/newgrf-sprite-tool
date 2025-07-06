#include "mytec/model/tools/view.hpp"

#include "mytec/model/image.hpp"

#include <SFML/Window/Mouse.hpp>
#include <imgui.h>

namespace mytec
{
view::view() : tool_t(tool_id::view) {}

bool view::is_engaged() const noexcept { return drag_start_delta_ && ImGui::IsMouseDown(0); }

void view::update(image& _image, const editor&)
{
    const auto dragging = ImGui::IsMouseDragging(0, 0);
    const auto hovered = ImGui::IsItemHovered();
    if (ImGui::IsMouseClicked(0))
    {
        drag_start_delta_.reset();
        if (hovered)
            drag_start_delta_ = _image.position_ - vf2(sf::Mouse::getPosition());
    }
    if (dragging & drag_start_delta_.has_value())
    {
        // ImGui::GetMouseDragDelta() works weird if user is simultaneously scrolling wheel TODO: Create issue
        _image.position_ = *drag_start_delta_ + vf2(sf::Mouse::getPosition());
    }
    if (dragging | !hovered)
        return;

    const auto& io = ImGui::GetIO();
    const float mw_delta = io.MouseWheel;
    if (mw_delta == 0)
        return;

    const auto center = io.MousePos - ImGui::GetItemRectMin();
    mw_delta > 0 ? _image.zoom_in(center) : _image.zoom_out(center);
}
} // namespace mytec
