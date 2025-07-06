#ifndef MYTEC_MENU_BAR_HPP
#define MYTEC_MENU_BAR_HPP

#include "mytec/model/tools/tool_t.hpp"
#include "mytec/ui/style.hpp"

namespace mytec
{
class event;

class menu_bar
{
public:
    menu_bar(style& _style);

    void update(event& _event, bool _usable, const std::unique_ptr<tool_t>& _tool) const;

    style& style_;
};
} // namespace mytec

#endif // MYTEC_MENU_BAR_HPP