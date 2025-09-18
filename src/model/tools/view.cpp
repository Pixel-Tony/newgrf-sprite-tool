#include "view.hpp"

#include "model/tools/tool.hpp"
#include "widgets/editor.hpp"

namespace mytec
{
view::view() : tool(tool::view) {}

void view::enter_editor(editor* _editor)
{
    if (_editor)
        _editor->setDragMode(QGraphicsView::ScrollHandDrag);
}

void view::exit_editor(editor* _editor)
{
    if (_editor)
        _editor->setDragMode(QGraphicsView::NoDrag);
}

bool view::editor_event(QEvent& _ev, editor&)
{
    switch (_ev.type())
    {
    case QEvent::MouseButtonPress:
        is_dragging_ = true;
        _ev.setAccepted(false); // leave for QGraphicsView to handle
        break;

    case QEvent::MouseButtonRelease:
        is_dragging_ = false;
        _ev.setAccepted(false); // leave for QGraphicsView to handle
        break;

    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::ShortcutOverride:
    case QEvent::Wheel:
        _ev.setAccepted(is_dragging_);
        break;

    default:
        _ev.ignore();
        break;
    }
    return _ev.isAccepted();
}
} // namespace mytec