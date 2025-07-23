#include "palette_tab_sprite.hpp"

#include "model/palette.hpp"

namespace
{
constexpr auto size = 16;
constexpr auto width = 16;
constexpr auto height = 20;
constexpr auto gap = 1;

QPixmap generate_sprite(const mytec::palette* const _plt)
{
    const auto a = width + gap;
    const auto b = height + gap;

    QPixmap result = {gap + 16 * a, gap + 16 * b};
    result.fill({40, 40, 40});

    QPainter painter{&result};
    for (int y = 0; y < 16; ++y)
        for (int x = 0; x < 16; ++x)
            painter.fillRect(QRect{gap + a * x, gap + b * y, width, height}, _plt->get(x, y));
    return result;
}
} // namespace

namespace mytec
{
palette_tab_sprite::palette_tab_sprite(palette::type _type, QWidget* _parent)
    : QLabel(_parent),
      palette_(palette::make(_type)),
      base_(generate_sprite(palette_))
{
    setPixmap(base_);
    setMouseTracking(true);
}

void palette_tab_sprite::enterEvent(QEnterEvent* _ev) { update_hovered(get_hovered_color(_ev->position())); }

void palette_tab_sprite::leaveEvent(QEvent*) { update_hovered(Qt::transparent); }

void palette_tab_sprite::mouseReleaseEvent(QMouseEvent* _ev)
{
    const auto color = get_hovered_color(_ev->position());
    if (color == Qt::transparent || pressed_color_ != color)
        return;
    emit color_selected(color, _ev->button() == Qt::LeftButton);
}

void palette_tab_sprite::mouseMoveEvent(QMouseEvent* _ev) { update_hovered(get_hovered_color(_ev->position())); }

void palette_tab_sprite::mousePressEvent(QMouseEvent* _ev) { pressed_color_ = get_hovered_color(_ev->position()); }

QColor palette_tab_sprite::get_hovered_color(QPointF _p)
{
    const auto px = static_cast<int>(_p.x());
    const auto py = static_cast<int>(_p.y());
    const auto a = ::width + ::gap;
    const auto b = ::height + ::gap;

    auto xd = px / a;
    auto xm = px % a;
    auto yd = py / b;
    auto ym = py % b;
    if (qMax(xd, yd) > 15 || qMin(xm, ym) < gap)
        return Qt::transparent;

    return palette_->get(xd, yd);
}

void palette_tab_sprite::update_hovered(const QColor _col)
{
    if (_col != hovered_color_)
        emit color_hovered(hovered_color_ = _col);
}
} // namespace mytec
