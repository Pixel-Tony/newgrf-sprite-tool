#include "palette_tab.hpp"

#include "model/palette.hpp"

namespace
{
constexpr auto pad = 0;
constexpr auto size = 16;
constexpr auto width = 13;
constexpr auto height = 15;
constexpr auto gap = 1;
} // namespace

namespace
{
using mytec::palette;

QPixmap generate_sprite(const palette::type _type)
{
    static const auto a = width + gap;
    static const auto b = height + gap;
    const auto* const plt = palette::make(_type);

    QPixmap result = {width + 15 * a + pad * 2, height + 15 * b + pad * 2};
    result.fill({40, 40, 40});

    QPainter painter{&result};
    for (int y = 0; y < size; ++y)
        for (int x = 0; x < size; ++x)
            painter.fillRect(QRect{pad + a * x, pad + b * y, width, height}, plt->get(x, y));
    return result;
}

QTabWidget* generate_tabs()
{
    auto* tabs = new QTabWidget;
    tabs->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    for (auto [pal, name] : {std::pair{palette::dos, "DOS"}, {palette::windows, "Windows"}})
    {
        auto* const pic = new QLabel;
        pic->setPixmap(generate_sprite(pal));

        auto* const lyt = new QVBoxLayout;
        lyt->setContentsMargins(4, 6, 4, 6);
        lyt->setSpacing(6);
        lyt->setAlignment(Qt::AlignTop | Qt::AlignCenter);
        lyt->addWidget(pic);

        auto* const tab = new QWidget;
        tab->setLayout(lyt);
        tabs->addTab(tab, name);
    }
    return tabs;
}
} // namespace

namespace mytec
{
palette_tab::palette_tab(QWidget* _parent) : QDockWidget("Palette", _parent)
{
    hide();
    setFeatures(QDockWidget::NoDockWidgetFeatures); // TODO
    body_ = generate_tabs();
    setWidget(body_);
}
} // namespace mytec