#ifndef MYTEC_PALETTE_HPP
#define MYTEC_PALETTE_HPP

#include <headers.hpp>

template <>
struct std::hash<QColor>
{
    std::size_t operator()(const QColor &_col);
};

namespace mytec
{
class palette
{
public:
    enum type
    {
        none,
        dos,
        windows,
        _count
    };

    static const palette *make(type _type);

    static const palette *find(const QList<QRgb> &_color_table);

    [[nodiscard]] const QVector<QRgb> &contents() const noexcept;
    [[nodiscard]] const QString &name() const noexcept;
    [[nodiscard]] bool has(QColor _color) const noexcept;
    [[nodiscard]] QColor get(int _x, int _y) const noexcept;

    const type type_;

private:
    explicit palette(type _type, const QString &_filename, QString &&_name);

    const QString name_;
    QVector<QRgb> contents_;
    QHash<QColor, uint> colors_;
};
} // namespace mytec

#endif // MYTEC_PALETTE_HPP