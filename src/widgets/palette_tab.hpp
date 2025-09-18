#ifndef MYTEC_PALETTE_TAB_HPP
#define MYTEC_PALETTE_TAB_HPP

#include "model/palette.hpp"

#include <headers.hpp>

namespace mytec
{
namespace priv
{
class plt_color_frame : public QFrame
{
    Q_OBJECT

public:
    plt_color_frame(QWidget* _parent = nullptr);

public slots:
    void set_color(QColor _new);

private:
    QFrame* color_box_;
    QLabel* color_id_;
};

class plt_sprite : public QLabel
{
    Q_OBJECT

public:
    plt_sprite(palette::type _type, QWidget* _parent = nullptr);

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
    QPoint last_press_coords_;
    QColor hovered_color_ = Qt::transparent;
    QColor pressed_color_ = Qt::transparent;
};
} // namespace priv

class palette_tab : public QDockWidget
{
    Q_OBJECT

public:
    palette_tab(QWidget* _parent = nullptr);

public slots:
    void set_palette(palette::type _type);
    void set_color(QColor _color, bool _primary);

signals:
    void color_selected(QColor _color, bool _primary);
    void color_hovered(QColor _color);

private:
    QTabWidget* const body_;
    QList<priv::plt_color_frame*> primary_frames_;
    QList<priv::plt_color_frame*> secondary_frames_;
};
} // namespace mytec

#endif // MYTEC_PALETTE_TAB_HPP