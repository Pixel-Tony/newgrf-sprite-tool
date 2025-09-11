#ifndef MYTEC_IMAGE_HPP
#define MYTEC_IMAGE_HPP

#include "palette.hpp"

namespace mytec
{
class image : public QImage
{
    Q_GADGET

public:
    image(QSize _size, palette::type _type);

    explicit image(const QString& _filepath);

    [[nodiscard]] const palette* get_palette() const noexcept;

    bool setPixelColor(const QPoint& _point, const QColor& _color);

    bool save_with_palette(const QString& _filepath, const char* _format = nullptr);

private:
    const palette* palette_;
};
} // namespace mytec

#endif // MYTEC_IMAGE_HPP