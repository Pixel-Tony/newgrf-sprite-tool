#ifndef MYTEC_IMAGE_HPP
#define MYTEC_IMAGE_HPP

#include "palette.hpp"

namespace mytec
{
class image : public QGraphicsObject
{
    Q_OBJECT

public:
    image(QSize _size, palette::type _type);

    explicit image(const QString& _filepath);

    [[nodiscard]] QImage& contents() noexcept;
    [[nodiscard]] const QImage& contents() const noexcept;
    [[nodiscard]] const palette* get_palette() const noexcept;

    [[nodiscard]] bool set_pixel_color(QPoint _point, QColor _color);
    [[nodiscard]] bool set_pixel_color(QPoint _point, QColor _color, QColor* _old);
    [[nodiscard]] bool save_with_palette(const QString& _filepath, const char* _format = nullptr) const;

signals:
    void mouse_moved(QGraphicsSceneMouseEvent& _ev);
    void mouse_pressed(QGraphicsSceneMouseEvent& _ev);
    void mouse_released(QGraphicsSceneMouseEvent& _ev);

protected:
    void paint(QPainter* _painter, const QStyleOptionGraphicsItem* _opt, QWidget* _widget = nullptr) override;
    [[nodiscard]] QRectF boundingRect() const override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* _ev) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* _ev) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* _ev) override;

private:
    QImage contents_;
    const palette* const palette_;
};
} // namespace mytec

#endif // MYTEC_IMAGE_HPP