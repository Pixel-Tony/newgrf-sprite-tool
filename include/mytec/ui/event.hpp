#ifndef MYTEC_EVENT_HPP
#define MYTEC_EVENT_HPP

#include "mytec/model/tools/tool_t.hpp"

#include <imgui.h>

namespace mytec
{
enum class event_type
{
    none = 0,
    create,
    open,
    save,
    save_as,
    close_image,
    choose_tool,
    undo,
    redo,
    swap_colors,
    quit,
    toggle_palette_visibility
};

struct event
{
    void make_on(bool _condition, event_type _type);
    void tool_on(bool _condition, tool_id _id);
    void on_shortcut(ImGuiKeyChord _value, event_type _type);
    void tool_on_shortcut(ImGuiKeyChord _value, tool_id _id);

    image* image_ = nullptr;
    event_type type_ = event_type::none;
    tool_id next_tool_ = tool_id::none;
};
} // namespace mytec

#endif // MYTEC_EVENT_HPP