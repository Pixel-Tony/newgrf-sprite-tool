#include "mytec/ui/components/dialogs.hpp"
#include "mytec/ui/misc.hpp"

#include <imgui.h>

namespace mytec
{
message_box::message_box(const char* _title, const char* _message) : popup_modal(_title, true, true), message_(_message) {}

void message_box::body()
{
    static constexpr vf2 size = {64, 32};

    ImGui::TextUnformatted(message_);
    ImGui::Dummy({0, 15});
    move_cursor_screen_pos((ImGui::GetContentRegionAvail() - size) * vf2{0.5f, 0});
    if (ImGui::Button("Ok", size) || ImGui::IsKeyPressed(ImGuiKey_Enter))
        close();
}
} // namespace mytec
