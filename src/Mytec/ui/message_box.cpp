#include "Mytec/util/vec.hpp"
#include "Mytec/ui/message_box.hpp"
#include "Mytec/ui/dialogs.hpp"
#include "Mytec/ui/misc.hpp"


namespace mytec::ui
{
message_box::message_box(const char *title, const char *message): popup_modal(title, true), message(message)
{
}

void message_box::body()
{
    static constexpr v2f size = {80, 24};

    ImGui::TextUnformatted(message);
    ImGui::Dummy({0, 15});
    move_cursor_screen_pos(v2f{ImGui::GetContentRegionAvail().x - size.x, 0} / 2.f);
    if (ImGui::Button("Ok", size))
    {
        close();
    }
}
}
