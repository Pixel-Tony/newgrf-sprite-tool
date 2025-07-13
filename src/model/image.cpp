#include "image.hpp"

namespace mytec
{
image::image(QSize _size, palette::type _type, QImage::Format _format)
    : QImage(_size, _format),
      palette_(palette::make(_type))
{
    fill(Qt::white);
}

const palette* image::get_palette() const noexcept { return palette_; }
} // namespace mytec
