#include "palette_tab.hpp"

namespace
{
constexpr auto width = 16;
constexpr auto height = 20;
constexpr auto gap = 1;
constexpr auto wg = width + gap;
constexpr auto hg = height + gap;

QPixmap generate_sprite(const mytec::palette* const _plt)
{
    QPixmap result = {gap + 16 * wg, gap + 16 * hg};
    result.fill({40, 40, 40});
    QPainter painter{&result};
    for (int y = 0; y < 16; ++y)
        for (int x = 0; x < 16; ++x)
            painter.fillRect(QRect{gap + wg * x, gap + hg * y, width, height}, _plt->get(x, y));
    return result;
}
} // namespace

namespace mytec
{
namespace priv
{
plt_color_frame::plt_color_frame(QWidget* _parent) : QFrame(_parent), color_box_(new QFrame), color_id_(new QLabel)
{
    color_box_->setFrameShape(Shape::Panel);
    color_box_->setLineWidth(2);
    color_box_->setFrameShadow(QFrame::Raised);
    color_box_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    color_box_->setFixedHeight(24);
    auto* lyt = new QVBoxLayout;
    lyt->setContentsMargins({});
    lyt->setSpacing(8);
    lyt->addWidget(color_box_);
    lyt->addWidget(color_id_);
    setLayout(lyt);
}

void plt_color_frame::set_color(QColor _new)
{
    color_ = _new;
    color_box_->setStyleSheet(tr("background: %0").arg(color_.name()));
    color_id_->setText(_new == Qt::transparent ? " * * * " : _new.name());
    update();
}

plt_sprite::plt_sprite(palette::type _type, QWidget* _parent)
    : QLabel(_parent),
      palette_(palette::make(_type)),
      base_(generate_sprite(palette_))
{
    setPixmap(base_);
    setMouseTracking(true);
}

void plt_sprite::enterEvent(QEnterEvent* _ev) { update_hovered(get_hovered_color(_ev->position())); }

void plt_sprite::leaveEvent(QEvent*) { update_hovered(Qt::transparent); }

void plt_sprite::mouseReleaseEvent(QMouseEvent* _ev)
{
    const auto color = get_hovered_color(_ev->position());
    if (color == Qt::transparent || pressed_color_ != color)
        return;
    emit color_selected(color, _ev->button() == Qt::LeftButton);
}

void plt_sprite::mouseMoveEvent(QMouseEvent* _ev) { update_hovered(get_hovered_color(_ev->position())); }

void plt_sprite::mousePressEvent(QMouseEvent* _ev) { pressed_color_ = get_hovered_color(_ev->position()); }

QColor plt_sprite::get_hovered_color(QPointF _p)
{
    const auto px = static_cast<int>(_p.x());
    const auto py = static_cast<int>(_p.y());

    auto xd = px / wg;
    auto xm = px % wg;
    auto yd = py / hg;
    auto ym = py % hg;
    if (qMax(xd, yd) > 15 || qMin(xm, ym) < gap)
        return Qt::transparent;
    return palette_->get(xd, yd);
}

void plt_sprite::update_hovered(const QColor _col)
{
    if (_col != hovered_color_)
        emit color_hovered(hovered_color_ = _col);
}
} // namespace priv

palette_tab::palette_tab(QWidget* _parent) : QDockWidget("Palette", _parent)
{
    using namespace priv;

    setFeatures(QDockWidget::NoDockWidgetFeatures); // TODO
    body_ = new QTabWidget;
    body_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    for (const auto [type, name] : {std::pair{palette::dos, "DOS"}, {palette::windows, "Windows"}})
    {
        auto* const sprite = new plt_sprite(type);
        connect(sprite, &plt_sprite::color_hovered, this, &palette_tab::color_hovered);
        connect(sprite, &plt_sprite::color_selected,
            [this](QColor _color, bool _primary)
            {
                (_primary ? primary_ : secondary_) = _color;
                emit colors_updated(primary_, secondary_);
            });

        auto* const hover = new plt_color_frame;
        connect(this, &palette_tab::color_hovered, hover, &plt_color_frame::set_color);

        auto* const primary = new plt_color_frame;
        auto* const secondary = new plt_color_frame;

        connect(this, &palette_tab::colors_updated,
            [=](QColor _primary, QColor _secondary)
            {
                primary->set_color(_primary);
                secondary->set_color(_secondary);
            });

        auto* const lyt = new QGridLayout;
        lyt->setContentsMargins(4, 6, 4, 6);
        lyt->setAlignment(Qt::AlignTop | Qt::AlignCenter);
        lyt->setSpacing(8);
        lyt->setVerticalSpacing(20);
        lyt->setColumnStretch(0, 3);
        lyt->setColumnStretch(1, 2);
        lyt->setColumnStretch(2, 3);
        lyt->addWidget(sprite, 0, 0, 1, 3);
        lyt->addWidget(primary, 1, 0);
        lyt->addWidget(hover, 1, 1);
        lyt->addWidget(secondary, 1, 2);

        auto* const tab = new QWidget;
        tab->setLayout(lyt);
        body_->addTab(tab, name);
    }
    setWidget(body_);
    emit color_hovered(Qt::transparent);
}

void palette_tab::set_palette(palette::type _type)
{
    if (_type != palette::none)
        body_->setCurrentIndex(_type - palette::dos);
}

void palette_tab::swap_colors()
{
    std::swap(primary_, secondary_);
    emit colors_updated(primary_, secondary_);
}

void palette_tab::bootstrap() { emit colors_updated(primary_, secondary_); }
} // namespace mytec