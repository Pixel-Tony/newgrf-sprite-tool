#ifndef MYTEC_TOOL_HPP
#define MYTEC_TOOL_HPP

#include <headers.hpp>

namespace mytec
{
class canvas;
class editor;
class image;

struct tool
{
    enum type
    {
        view,
        pen
    };

    static tool* current;

    tool(type _type);

    virtual ~tool() = default;

    virtual bool editor_event(QEvent& _ev, editor& _editor);
    virtual void enter_editor(editor* _editor);
    virtual void exit_editor(editor* _editor);
    virtual void image_mouse_press_event(QGraphicsSceneMouseEvent& _ev, canvas& _canv, editor& _ed);
    virtual void image_mouse_release_event(QGraphicsSceneMouseEvent& _ev, canvas& _canv, editor& _ed);
    virtual void image_mouse_move_event(QGraphicsSceneMouseEvent& _ev, canvas& _canv, editor& _ed);

    const type type_;
};
} // namespace mytec

#endif // MYTEC_TOOL_HPP