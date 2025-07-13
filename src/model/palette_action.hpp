#ifndef MYTEC_PALETTE_BUTTON_HPP
#define MYTEC_PALETTE_BUTTON_HPP
#include <qcolor.h>
#include <qevent.h>
#include <qrgb.h>
#include <qtoolbutton.h>
#include <qwidget.h>
#include <qwidgetaction.h>

namespace mytec
{
class palette_action : public QWidgetAction
{
    Q_OBJECT

public:
    explicit palette_action(QObject* _parent = nullptr);

public slots:
    void change_colors(QColor _primary, QColor _secondary);
};
} // namespace mytec

#endif // MYTEC_PALETTE_BUTTON_HPP