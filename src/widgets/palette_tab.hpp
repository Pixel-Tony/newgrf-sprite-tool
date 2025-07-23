#ifndef MYTEC_PALETTE_TAB_HPP
#define MYTEC_PALETTE_TAB_HPP

#include <headers.hpp>

namespace mytec
{
class palette_tab : public QDockWidget
{
    Q_OBJECT

public:
    palette_tab(QWidget* _parent = nullptr);

    void bootstrap();

public slots:
    void swap_colors();

signals:
    void colors_updated(QColor _primary, QColor _secondary);
    void color_hovered(QColor _color);

private:
    QTabWidget* body_;

    QColor primary_ = Qt::white;
    QColor secondary_ = Qt::blue;
};
} // namespace mytec

#endif // MYTEC_PALETTE_TAB_HPP