#ifndef MYTEC_VIEW_HPP
#define MYTEC_VIEW_HPP

#include "tool.hpp"

#include <headers.hpp>

namespace mytec
{
class view final : public tool
{
public:
    view();

    bool editor_event(QEvent& _ev, editor& _editor) override;

    void enter_editor(editor* _editor) override;
    void exit_editor(editor* _editor) override;

private:
    bool is_dragging_ = false;
};
} // namespace mytec

#endif // MYTEC_VIEW_HPP