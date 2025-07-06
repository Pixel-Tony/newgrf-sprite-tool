#include "mytec/ui/misc.hpp"

#include <imgui.h>

namespace mytec
{
void move_cursor_screen_pos(const vf2 _delta) { ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() + _delta); }
} // namespace mytec
