#include "palette.hpp"

#include <array>

namespace
{
using namespace mytec;

std::array<palette *, palette::_count> palettes = {nullptr};
auto **p_none = &palettes[palette::none];
auto **p_dos = &palettes[palette::dos];
auto **p_win = &palettes[palette::windows];
} // namespace

namespace mytec
{
const palette *palette::make(palette::type _type)
{
    if (*p_dos == nullptr)
    {
        *p_dos = new palette(type::dos, ":/assets/palette-dos.act", "DOS");
        *p_win = new palette(type::windows, ":/assets/palette-win.act", "Windows");
    }
    return palettes[_type];
}

const palette *palette::find(const QList<QRgb> &_color_table)
{
    if (std::ranges::equal(_color_table, (*p_dos)->contents()))
        return *p_dos;
    if (std::ranges::equal(_color_table, (*p_win)->contents()))
        return *p_win;
    return nullptr;
}

const QVector<QRgb> &palette::contents() const noexcept { return contents_; }

const QString &palette::name() const noexcept { return name_; }

bool palette::has(QColor _color) const noexcept { return colors_.contains(_color); }

QColor palette::get(int _x, int _y) const noexcept { return contents_[_y * 16 + _x]; }

palette::palette(const type _type, const QString &_filename, QString &&_name)
    : type_(_type),
      name_(_name),
      contents_(256)
{
    const auto *const ptr = QResource{_filename}.data();
    for (int i = 0; i < 256; ++i)
        contents_[i] = qRgb(ptr[3 * i], ptr[3 * i + 1], ptr[3 * i + 2]);

    for (int i = 0; auto c : contents_)
        colors_[c] = i++;
}

bool palette::color_comparer::operator()(QColor _a, QColor _b) const { return _a.rgba() < _b.rgba(); }
} // namespace mytec