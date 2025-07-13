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

private:
    QTabWidget* body_;
};
} // namespace mytec

#endif // MYTEC_PALETTE_TAB_HPP