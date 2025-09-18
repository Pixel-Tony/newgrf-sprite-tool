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

std::size_t std::hash<QColor>::operator()(const QColor &_col) { return _col.rgba(); }

namespace mytec
{
const palette *palette::make(palette::type _type)
{
    if (*p_dos == nullptr)
    {
        *p_dos = new palette(type::dos, ":/assets/palette-dos.act", ":/assets/palette-dos-groups.json", "DOS");
        *p_win = new palette(type::windows, ":/assets/palette-win.act", ":/assets/palette-win-groups.json", "Windows");
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

uint palette::index(QColor _color) const noexcept { return colors_.value(_color, -1); }

QColor palette::get(int _x, int _y) const noexcept { return contents_[_y * 16 + _x]; }

palette::palette(const type _type, const QString &_filename, const QString &_groups_filename, QString &&_name)
    : type_(_type),
      name_(_name),
      contents_(256)
{
    const auto *const ptr = QResource{_filename}.data();
    for (int i = 0; i < 256; ++i)
        contents_[i] = qRgb(ptr[3 * i], ptr[3 * i + 1], ptr[3 * i + 2]);

    for (int i = 0; auto c : contents_)
        colors_[c] = i++;

    const QJsonDocument doc = QJsonDocument::fromJson(QResource(_groups_filename).uncompressedData());
    for (const auto &key : doc.object().keys())
    {
        auto arr = doc[key].toArray();
        uint8_t begin = 0;
        uint8_t end = 0;
        switch (arr.count())
        {
        case 1:
            begin = end = arr[0].toString().toUInt(nullptr, 16);
            break;
        case 2:
            begin = arr[0].toString().toUInt(nullptr, 16);
            end = arr[1].toString().toUInt(nullptr, 16);
            break;
        default:
            throw QJsonParseError();
        }
        color_groups_.push_back({QString("Group: %0").arg(key), {begin, end}});
    }
}

const QString &palette::color_group(QColor _color) const noexcept
{
    static const QString none = "Group: none";
    static const QString empty = "";
    if (_color == Qt::transparent)
        return empty;
    if (!colors_.contains(_color))
        return none;
    const auto ind = colors_.value(_color);
    for (const auto &[name, bounds] : color_groups_)
    {
        if (bounds.first <= ind && ind <= bounds.second)
            return name;
    }
    return none;
}
} // namespace mytec