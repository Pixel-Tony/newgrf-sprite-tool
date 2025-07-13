#include "palette.hpp"

#include <array>
#include <qcolor.h>
#include <qobject.h>
#include <qresource.h>

namespace
{
std::array<QColor, 256> convert(const QString& _filename)
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
} // namespace

namespace mytec
{
const palette* palette::make(palette::type _type)
{
    static const palette dos{":/assets/palette-dos.act", "DOS"};
    static const palette win{":/assets/palette-win.act", "Windows"};
    static constexpr std::array<const palette*, 3> _choice{{nullptr, &dos, &win}};
    return _choice[static_cast<int>(_type)];
}

const QString& palette::name() const noexcept { return name_; }

QColor palette::get(int _x, int _y) const noexcept { return contents_[_y * 16 + _x]; }

palette::palette(const QString& _filename, QString&& _name) : name_(_name), contents_(convert(_filename)) {}
} // namespace mytec