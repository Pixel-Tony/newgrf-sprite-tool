#include "view.hpp"

#include "model/tools/tool.hpp"
#include "widgets/editor.hpp"

#include <limits>
#include <qcoreevent.h>
#include <qcursor.h>
#include <qevent.h>
#include <qnamespace.h>

namespace
{
const QPoint drag_start_none = -QPoint{1, 1} * std::numeric_limits<int>::max();
}

namespace mytec
{
view::view() : tool(tool::view), start_drag_delta_(drag_start_none) {}

bool view::event(QEvent* _ev, editor* _editor, image*)
{
    switch (_ev->type())
    {
    case QEvent::MouseButtonPress:
    {
        auto* const mev = static_cast<QMouseEvent*>(_ev);
        start_drag_delta_ = drag_start_none;
        _ev->setAccepted(mev->button() == Qt::LeftButton);
        if (_ev->isAccepted())
        {
            _editor->setCursor({Qt::CursorShape::DragMoveCursor});
            start_drag_delta_ = _editor->image_pos() - QCursor::pos();
        }
        break;
    }

    case QEvent::MouseMove:
        _ev->setAccepted(start_drag_delta_ != drag_start_none);
        if (_ev->isAccepted())
            _editor->set_image_pos(QCursor::pos() + start_drag_delta_);
        break;

    case QEvent::MouseButtonRelease:
        _ev->setAccepted(start_drag_delta_ != drag_start_none);
        start_drag_delta_ = drag_start_none;
        _editor->setCursor({Qt::CursorShape::ArrowCursor});
        break;

    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::ShortcutOverride:
    case QEvent::Wheel:
        _ev->setAccepted(start_drag_delta_ != drag_start_none);
        break;

    default:
        _ev->ignore();
        break;
    }
    return _ev->isAccepted();
}
} // namespace mytec