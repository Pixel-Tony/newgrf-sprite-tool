#include "mytec/ui/event.hpp"

namespace mytec
{
void event::make_on(bool _condition, event_type _type)
{
    if (_condition && type_ == event_type::none)
        type_ = _type;
}

void event::tool_on(bool _condition, tool_id _id)
{
    if (_condition && next_tool_ == tool_id::none)
    {
        type_ = event_type::choose_tool;
        next_tool_ = _id;
    }
}

void event::on_shortcut(ImGuiKeyChord _value, event_type _type) { make_on(ImGui::IsKeyChordPressed(_value), _type); }

void event::tool_on_shortcut(ImGuiKeyChord _value, tool_id _id) { tool_on(ImGui::IsKeyChordPressed(_value), _id); }
} // namespace mytec