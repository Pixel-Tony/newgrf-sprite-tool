#ifndef MYTEC_PROMISE_POPUP_INL
#define MYTEC_PROMISE_POPUP_INL
#include "promise_popup.hpp"

namespace mytec
{
template <class... Ts>
promise_popup<Ts...>::promise_popup(const char* _title, callback_t&& _bound, bool _has_cross, bool _close_on_quit)
    : popup_modal(_title, _has_cross, _close_on_quit),
      bound_(std::forward<callback_t>(_bound))
{
}

template <class... Ts>
void promise_popup<Ts...>::resolve(Ts&&... _args)
{
    if (bound_)
        std::invoke(std::forward<callback_t>(bound_), std::move(_args)...);
    close();
}
} // namespace mytec

#endif // MYTEC_PROMISE_POPUP_INL