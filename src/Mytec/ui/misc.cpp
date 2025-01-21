#include "Mytec/ui/misc.hpp"


namespace mytec::ui
{
void move_cursor_screen_pos(const v2f delta)
{
    ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + delta);
}
}
