#ifndef MYTEC_PALETTE_TAB_COLOR_HPP
#define MYTEC_PALETTE_TAB_COLOR_HPP

#include <headers.hpp>

namespace mytec
{
class palette_tab_color : public QFrame
{
    Q_OBJECT

public:
    palette_tab_color(QWidget* _parent = nullptr);

public slots:
    void set_color(QColor _new);

private:
    QColor color_ = Qt::black;
};
} // namespace mytec

#endif // MYTEC_PALETTE_TAB_COLOR_HPP