#ifndef MYTEC_PALETTE_HPP
#define MYTEC_PALETTE_HPP

#include <array>
#include <headers.hpp>

namespace mytec
{
class palette
{
public:
    enum type
    {
        none,
        dos,
        windows
    };

    static const palette* make(type _type);

    [[nodiscard]] const QString& name() const noexcept;

    [[nodiscard]] QColor get(int _x, int _y) const noexcept;

private:
    explicit palette(const QString& _filename, QString&& _name);

    const QString name_;
    const std::array<QColor, 256> contents_;
};
} // namespace mytec

#endif // MYTEC_PALETTE_HPP