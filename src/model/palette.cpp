#include "palette.hpp"

#include <array>

namespace
{
auto convert(const QString& _filename)
{
    std::array<QColor, 256> result;
    const auto* const ptr = QResource{_filename}.data();
    for (int i = 0; auto& dest : result)
    {
        dest = QColor{ptr[i], ptr[i + 1], ptr[i + 2]};
        i += 3;
    }
    return result;
}

auto map_from_contents(const std::array<QColor, 256>& _source)
{
    std::map<QColor, uint, mytec::ColorCompare> result;
    int i = 0;
    for (auto c : _source)
        result[c] = i++;
    return result;
}
} // namespace

namespace mytec
{
bool ColorCompare::operator()(QColor _a, QColor _b) const { return _a.rgba() < _b.rgba(); }

const palette* palette::make(palette::type _type)
{
    static const palette p_dos{":/assets/palette-dos.act", "DOS"};
    static const palette p_win{":/assets/palette-win.act", "Windows"};
    static constexpr std::array<const palette*, 3> _choice{{nullptr, &p_dos, &p_win}};
    return _choice[_type];
}

const QString& palette::name() const noexcept { return name_; }

bool palette::has(QColor _color) const noexcept { return colors_.contains(_color); }

QColor palette::get(int _x, int _y) const noexcept { return contents_[_y * 16 + _x]; }

palette::palette(const QString& _filename, QString&& _name)
    : name_(_name),
      contents_(convert(_filename)),
      colors_(map_from_contents(contents_))
{
}
} // namespace mytec