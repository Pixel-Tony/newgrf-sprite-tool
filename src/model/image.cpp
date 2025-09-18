#include "image.hpp"

namespace mytec
{
image::image(QSize _size, palette::type _type) : contents_(_size, QImage::Format_ARGB32), palette_(palette::make(_type))
{
    contents_.fill(Qt::white);
}

image::image(const QString& _filepath) : contents_(_filepath, "png"), palette_(palette::find(contents_.colorTable()))
{
    if (contents_.isNull())
        throw std::runtime_error{"File is not an image."};
    if (contents_.format() != QImage::Format_ARGB32)
        contents_.convertTo(QImage::Format_ARGB32);
}

bool image::set_pixel_color(const QPoint _point, const QColor _color, QColor* _old)
{
    if (palette_ && !palette_->has(_color))
    {
        QToolTip::showText(QCursor::pos(), "Color not in the image palette");
        return false;
    }
    if ((*_old = contents_.pixelColor(_point)) == _color)
        return false;
    contents_.setPixelColor(_point, _color);
    update();
    return true;
}

bool image::set_pixel_color(const QPoint _point, const QColor _color)
{
    QColor tmp;
    return set_pixel_color(_point, _color, &tmp);
}

void image::paint(QPainter* _painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    const auto rect_ = QRectF({}, contents_.size());
    if (!palette_)
    {
        static QPixmap tex;
        if (tex.isNull())
        {
            const auto gray = qRgb(220, 220, 220);
            tex = QPixmap(4, 4);
            tex.fill(Qt::white);
            QPainter p(&tex);
            p.fillRect(QRect(0, 0, 2, 2), gray);
            p.fillRect(QRect(2, 2, 2, 2), gray);
        }
        _painter->drawTiledPixmap(rect_, tex);
    }
    _painter->drawImage(rect_, contents_);
}

bool image::save_with_palette(const QString& _filepath, const char* _format) const
{
    return (palette_ == nullptr ? contents_ : contents_.convertToFormat(QImage::Format_Indexed8, palette_->contents()))
        .save(_filepath, _format);
}

void image::mouseMoveEvent(QGraphicsSceneMouseEvent* _ev) { emit mouse_moved(*_ev); }

void image::mousePressEvent(QGraphicsSceneMouseEvent* _ev) { emit mouse_pressed(*_ev); }

void image::mouseReleaseEvent(QGraphicsSceneMouseEvent* _ev) { emit mouse_released(*_ev); }

QRectF image::boundingRect() const
{
    auto size = contents_.size().toSizeF();
    return {0, 0, size.width(), size.height()};
}

QImage& image::contents() noexcept { return contents_; }

const QImage& image::contents() const noexcept { return contents_; }

const palette* image::get_palette() const noexcept { return palette_; }
} // namespace mytec
