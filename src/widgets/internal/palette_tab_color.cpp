#include "palette_tab_color.hpp"

namespace mytec
{
palette_tab_color::palette_tab_color(QWidget* _parent) : QFrame(_parent)
{
    setFrameShape(Shape::Panel);
    setLineWidth(2);
    setFrameShadow(QFrame::Raised);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(24);
}

void palette_tab_color::set_color(QColor _new)
{
    color_ = _new;
    setStyleSheet(tr("background: %0").arg(color_.name()));
    update();
}
} // namespace mytec
