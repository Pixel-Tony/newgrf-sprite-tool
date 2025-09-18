#ifndef MYTEC_PALETTE_BUTTON_HPP
#define MYTEC_PALETTE_BUTTON_HPP

#include <headers.hpp>

namespace mytec
{
class palette_action : public QWidgetAction
{
    Q_OBJECT

public:
    explicit palette_action(QObject* _parent = nullptr);

public slots:
    void set_colors(QColor _primary, QColor _secondary);
};
} // namespace mytec

#endif // MYTEC_PALETTE_BUTTON_HPP