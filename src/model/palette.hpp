#ifndef MYTEC_PALETTE_HPP
#define MYTEC_PALETTE_HPP

#include <headers.hpp>
#include <map>

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

    static const palette* make(type _type);

    static const palette* find(const QList<QRgb>& _color_table);

    [[nodiscard]] const QVector<QRgb>& contents() const noexcept;
    [[nodiscard]] const QString& name() const noexcept;
    [[nodiscard]] bool has(QColor _color) const noexcept;
    [[nodiscard]] QColor get(int _x, int _y) const noexcept;

    const type type_;

private:
    struct color_comparer
    {
        bool operator()(QColor _a, QColor _b) const;
    };

    explicit palette(type _type, const QString& _filename, QString&& _name);

    const QString name_;
    QVector<QRgb> contents_;
    std::map<QColor, uint, color_comparer> colors_;
};
} // namespace mytec

#endif // MYTEC_PALETTE_HPP