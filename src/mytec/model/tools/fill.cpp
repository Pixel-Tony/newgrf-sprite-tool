#include "mytec/model/tools/fill.hpp"

#include "mytec/model/editor.hpp"
#include "mytec/model/image.hpp"

namespace mytec
{
fill::fill() : tool_t(tool_id::fill) {}

bool fill::is_engaged() const noexcept
{
    const auto& clicks = ImGui::GetIO().MouseClicked;
    return clicks[0] || clicks[1];
}

void fill::update(image& _target, const editor& _editor)
{
    (void)_target;
    (void)_editor;
    // TODO
    // const auto click = _editor.get_click_info();
    // const auto mouse_pos = _target.get_pixel_under_mouse();
    // if (!click || !mouse_pos)
    // {
    //     return;
    // }
    // auto [button, color] = *click;
    // auto pos = *mouse_pos;
}
} // namespace mytec