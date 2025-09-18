#include "tool.hpp"

namespace mytec
{
tool::tool(tool::type _type) : type_(_type) {}

tool* tool::current = nullptr;

bool tool::editor_event(QEvent&, editor&) { return false; }

void tool::enter_editor(editor*) {}

void tool::exit_editor(editor*) {}

void tool::image_mouse_press_event(QGraphicsSceneMouseEvent& _ev, canvas&, editor&) { _ev.ignore(); }

void tool::image_mouse_release_event(QGraphicsSceneMouseEvent& _ev, canvas&, editor&) { _ev.ignore(); }

void tool::image_mouse_move_event(QGraphicsSceneMouseEvent& _ev, canvas&, editor&) { _ev.ignore(); }
} // namespace mytec