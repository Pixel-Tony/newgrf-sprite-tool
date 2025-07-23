#include "palette_tab.hpp"

#include "internal/palette_tab_color.hpp"
#include "internal/palette_tab_sprite.hpp"

namespace mytec
{
using sprite_t = palette_tab_sprite;

palette_tab::palette_tab(QWidget* _parent) : QDockWidget("Palette", _parent)
{
    hide();
    setFeatures(QDockWidget::NoDockWidgetFeatures); // TODO

    body_ = new QTabWidget;
    body_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    for (const auto [type, name] : {std::pair{palette::dos, "DOS"}, {palette::windows, "Windows"}})
    {
        auto* const sprite = new sprite_t(type);
        auto* const primary = new palette_tab_color;
        auto* const secondary = new palette_tab_color;
        auto* const hover = new palette_tab_color;

        auto* const primary_label = new QLabel;
        auto* const secondary_label = new QLabel;

        auto* const lyt = new QGridLayout;
        lyt->setContentsMargins(4, 6, 4, 6);
        lyt->setHorizontalSpacing(6);
        lyt->setVerticalSpacing(9);
        lyt->setAlignment(Qt::AlignTop | Qt::AlignCenter);
        lyt->setColumnStretch(0, 2);
        lyt->setColumnStretch(1, 1);
        lyt->setColumnStretch(2, 2);

        lyt->addWidget(sprite, 0, 0, 1, 3);
        lyt->addWidget(primary, 1, 0);
        lyt->addWidget(hover, 1, 1);
        lyt->addWidget(secondary, 1, 2);
        lyt->addWidget(primary_label, 2, 0);
        lyt->addWidget(secondary_label, 2, 2);

        connect(sprite, &sprite_t::color_hovered, this, &palette_tab::color_hovered);
        connect(this, &palette_tab::color_hovered, hover, &palette_tab_color::set_color);

        connect(sprite, &sprite_t::color_selected,
            [this](QColor _color, bool _primary)
            {
                (_primary ? primary_ : secondary_) = _color;
                emit colors_updated(primary_, secondary_);
            });
        connect(this, &palette_tab::colors_updated,
            [=](QColor _primary, QColor _secondary)
            {
                primary->set_color(_primary);
                primary_label->setText(_primary.name(QColor::NameFormat::HexRgb));
                secondary->set_color(_secondary);
                secondary_label->setText(_secondary.name(QColor::NameFormat::HexRgb));
            });

        auto* const tab = new QWidget;
        tab->setLayout(lyt);
        body_->addTab(tab, name);
    }
    setWidget(body_);
    emit color_hovered(Qt::transparent);
}

void palette_tab::swap_colors()
{
    const auto tmp = primary_;
    primary_ = secondary_;
    secondary_ = tmp;
    emit colors_updated(primary_, secondary_);
}

void palette_tab::bootstrap() { emit colors_updated(primary_, secondary_); }
} // namespace mytec