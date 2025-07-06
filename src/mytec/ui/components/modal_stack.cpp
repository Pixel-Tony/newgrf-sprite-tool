#include "mytec/ui/components/modal_stack.hpp"

#include "mytec/ui/popup_modal.hpp"

namespace mytec
{

void modal_stack::update()
{
    std::erase_if(*this, [](auto& _it) { return !_it->update(); });
}

void modal_stack::on_exit()
{
    while (size() > 0)
    {
        if (const auto& last = back(); !last->close_on_quit_)
            break;
        pop_back();
    }
}

} // namespace mytec