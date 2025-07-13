#ifndef MYTEC_VIEW_HPP
#define MYTEC_VIEW_HPP
#include "tool.hpp"

#include <qpoint.h>

namespace mytec
{
class view final : public tool
{
public:
    view();

    bool event(QEvent* _ev, editor* _editor, image* _image) override;

private:
    QPoint start_drag_delta_;
};
} // namespace mytec

#endif // MYTEC_VIEW_HPP