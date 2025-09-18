#include "pen.hpp"

#include "model/image.hpp"
#include "widgets/canvas.hpp"
#include "widgets/editor.hpp"

#include <ranges>

namespace mytec
{
pen::command::command(QColor _color, image* _image) : color_(_color), image_(_image) {}

void pen::command::undo()
{
    for (const auto& [pos, old_color] : pixels_)
        (void)image_->set_pixel_color(pos, old_color);
}

void pen::command::redo()
{
    for (const auto& pos : pixels_ | std::views::keys)
        (void)image_->set_pixel_color(pos, color_);
}

void pen::command::add_color(QPointF _pos)
{
    if (!image_->contains(_pos))
        return;
    const auto pixel_pos = QPoint(_pos.x(), _pos.y());
    QColor old;
    if (image_->set_pixel_color(pixel_pos, color_, &old))
        pixels_.emplace_back(pixel_pos, old);
}

pen::pen() : tool(tool::pen) {}

pen::~pen() { delete command_; }

void pen::image_mouse_move_event(QGraphicsSceneMouseEvent& _ev, canvas&, editor&)
{
    _ev.setAccepted(command_);
    if (command_)
        command_->add_color(_ev.pos());
}

void pen::image_mouse_press_event(QGraphicsSceneMouseEvent& _ev, canvas& _canv, editor& _ed)
{
    const auto pos = _ev.pos();
    auto& img = _ed.get_image();
    if (!img.contains(pos))
        return;

    QColor color;
    switch (_ev.button())
    {
    case Qt::MiddleButton:
    {
        _ev.accept();
        const auto pixel = img.contents().pixelColor(QPoint(pos.x(), pos.y()));
        emit _canv.set_color(pixel, true);
        return;
    }
    case Qt::LeftButton:
        color = _canv.color(true);
        break;
    case Qt::RightButton:
        color = _canv.color(false);
        break;
    default:
        _ev.ignore();
        return;
    }
    command_ = new command(color, &img);
    command_->add_color(pos);
    _ev.accept();
}

void pen::image_mouse_release_event(QGraphicsSceneMouseEvent& _ev, canvas&, editor& _ed)
{
    _ev.setAccepted(command_);
    if (command_ && !command_->pixels_.empty())
        _ed.history()->push(command_);
    else
        delete command_;
    command_ = nullptr;
}

bool pen::editor_event(QEvent& _ev, editor&)
{
    switch (_ev.type())
    {
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::ShortcutOverride:
    case QEvent::Wheel:
        _ev.setAccepted(command_);
        break;

    default:
        _ev.ignore();
        break;
    }
    return _ev.isAccepted();
}
} // namespace mytec