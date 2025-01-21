#include "Mytec/model/image.hpp"
#include "Mytec/tools/view.hpp"

#include "Mytec/model/editor.hpp"


namespace
{
constexpr mytec::v2f drag_start_pos_none{FLT_MAX, FLT_MAX};
}

namespace mytec::tools
{
view::view() : tool_t(), drag_start_pos(drag_start_pos_none)
{
}

tool_id view::id() const
{
    return tool_id::view;
}

void view::update(model::image &image, model::editor &)
{
    constexpr auto lmb = ImGuiMouseButton_Left;
    const bool moved = ImGui::IsMouseDragging(lmb, 0);
    const bool hovered = ImGui::IsItemHovered();
    if (ImGui::IsMouseClicked(lmb))
    {
        drag_start_pos = hovered ? image.position : drag_start_pos_none;
    }
    if (moved && drag_start_pos != drag_start_pos_none)
    {
        image.position = drag_start_pos + ImGui::GetMouseDragDelta(lmb, 0);
    }
    if (hovered && !moved)
    {
        const auto &io = ImGui::GetIO();
        if (const float ds = io.MouseWheel; ds != 0)
        {
            const auto center = io.MousePos - ImGui::GetItemRectMin();
            ds > 0 ? image.zoom_in(center) : image.zoom_out(center);
        }
    }
}

bool view::engaged() const
{
    return drag_start_pos != drag_start_pos_none && ImGui::IsMouseDown(ImGuiMouseButton_Left);
}
}
