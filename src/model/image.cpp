#include "image.hpp"

namespace mytec
{
image::image(QSize _size, palette::type _type) : QImage(_size, Format_ARGB32), palette_(palette::make(_type))
{
    fill(Qt::white);
}

image::image(const QString& _filepath) : QImage(_filepath, "png"), palette_(palette::find(colorTable()))
{
    if (isNull())
        throw std::runtime_error{"File is not an image."};
    if (format() != Format_ARGB32)
        convertTo(Format_ARGB32);
}

bool image::setPixelColor(const QPoint& _point, const QColor& _color)
{
    if (palette_ && !palette_->has(_color))
        return false;
    QImage::setPixelColor(_point, _color);
    return true;
}

bool image::save_with_palette(const QString& _filepath, const char* _format)
{
    QImage out = palette_ == nullptr ? *this : convertToFormat(Format_Indexed8, palette_->contents());
    return out.save(_filepath, _format);
}

const palette* image::get_palette() const noexcept { return palette_; }
} // namespace mytec
