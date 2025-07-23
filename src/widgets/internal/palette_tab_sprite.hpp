#ifndef MYTEC_PALETTE_TAB_SPRITE_HPP
#define MYTEC_PALETTE_TAB_SPRITE_HPP

#include "model/palette.hpp"

namespace mytec
{
class palette_tab_sprite : public QLabel
{
    Q_OBJECT

public:
    palette_tab_sprite(palette::type _type, QWidget* _parent = nullptr);

signals:
    void color_hovered(QColor _color);
    void color_selected(QColor _color, bool _primary);

protected:
    void enterEvent(QEnterEvent* _ev) override;
    void leaveEvent(QEvent* _ev) override;
    void mouseReleaseEvent(QMouseEvent* _ev) override;
    void mouseMoveEvent(QMouseEvent* _ev) override;
    void mousePressEvent(QMouseEvent* _ev) override;

private:
    QColor get_hovered_color(QPointF _p);
    void update_hovered(QColor _color);

    const mytec::palette* const palette_;
    const QPixmap base_;
    QPoint last_press_coords_;
    QColor hovered_color_ = Qt::transparent;
    QColor pressed_color_ = Qt::transparent;
};
} // namespace mytec

#endif // MYTEC_PALETTE_TAB_SPRITE_HPP