#ifndef MYTEC_PALETTE_TAB_HPP
#define MYTEC_PALETTE_TAB_HPP
#include "mytec/ui/style.hpp"

namespace mytec
{
class palette_tab
{
public:
    palette_tab(const style& _style);

    void update();

    const style& style_;
    bool is_open_ = false;
};
} // namespace mytec
#endif