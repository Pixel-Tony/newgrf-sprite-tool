#include "palette_action.hpp"

namespace mytec
{
palette_action::palette_action(QObject* _parent) : QWidgetAction(_parent) { setCheckable(true); }

void palette_action::change_colors(QColor _primary, QColor _secondary)
{
    QPixmap pix{{24, 24}};
    pix.fill(Qt::transparent);
    QPainter painter{&pix};
    painter.setPen(qRgba(0, 0, 0, 255));
    painter.setBrush(_secondary);
    painter.drawRect(1, 0, 16, 16);
    painter.setBrush(_primary);
    painter.drawRect(6, 7, 16, 16);
    setIcon(pix);
}
} // namespace mytec
