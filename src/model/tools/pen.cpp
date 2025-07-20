#include "pen.hpp"

#include "model/image.hpp"
#include "widgets/editor.hpp"

#include <ranges>

namespace mytec
{
pen::command::command(bool _left_btn, QColor _color, editor* _editor, image* _image)
    : left_btn_(_left_btn),
      color_(_color),
      editor_(_editor),
      image_(_image)
{
}

bool pen::command::is_empty() const noexcept { return pixels_.empty(); }

void pen::command::undo()
{
    for (const auto& [pos, old_color] : pixels_)
        image_->setPixelColor(pos, old_color);
    editor_->update();
}

void pen::command::redo()
{
    for (const auto& pos : pixels_ | std::views::keys)
        image_->setPixelColor(pos, color_);
    editor_->update();
}

void pen::command::append(QPoint _p, QColor _col) { pixels_.emplace_back(_p, _col); }

pen::pen() : tool(tool::pen) {}

pen::~pen() { delete command_; }

bool pen::event(QEvent& _ev, editor& _editor, image& _image)
{
    switch (_ev.type())
    {
    case QEvent::MouseButtonPress:
    {
        auto* const mev = static_cast<QMouseEvent*>(&_ev);
        const auto buttons = mev->buttons();
        _ev.setAccepted(buttons & (Qt::LeftButton | Qt::RightButton));
        if (!_ev.isAccepted())
            break;
        const auto is_left = buttons & Qt::LeftButton;
        const auto color = is_left ? _editor.primary() : _editor.secondary();
        command_ = new command(is_left, color, &_editor, &_image);
        const auto pixel = _editor.to_image_coords(mev->position());
        if (!pixel)
            break;
        const auto old = _editor.put_pixel(*pixel, command_->color_);
        if (!old)
            break;
        command_->append(*pixel, *old);
    }
    break;

    case QEvent::MouseMove:
    {
        const auto pos = static_cast<QMouseEvent*>(&_ev)->position();
        _ev.setAccepted(command_);
        if (!command_)
            break;
        const auto pixel = _editor.to_image_coords(pos);
        if (!pixel)
            break;
        const auto old = _editor.put_pixel(*pixel, command_->color_);
        if (!old)
            break;
        command_->append(*pixel, *old);
    }
    break;

    case QEvent::MouseButtonRelease:
        _ev.setAccepted(command_);
        if (command_ && !command_->is_empty())
            _editor.history()->push(command_);
        else
            delete command_;
        command_ = nullptr;
        break;

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