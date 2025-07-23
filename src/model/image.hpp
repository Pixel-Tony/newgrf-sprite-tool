#ifndef MYTEC_IMAGE_HPP
#define MYTEC_IMAGE_HPP

#include "palette.hpp"

namespace mytec
{
class image : public QImage
{
    Q_GADGET

public:
    image(QSize _size, palette::type _type, QImage::Format = QImage::Format_RGBA8888);

    [[nodiscard]] const palette* get_palette() const noexcept;

private:
    const palette* palette_;
};
} // namespace mytec

#endif // MYTEC_IMAGE_HPP