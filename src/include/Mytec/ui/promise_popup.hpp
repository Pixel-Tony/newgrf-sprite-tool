#ifndef MYTEC_UI_HPP
#define MYTEC_UI_HPP
#include "Mytec/ui/popup_modal.hpp"

#include <functional>


namespace mytec::ui
{
template <class T>
class promise_popup : public popup_modal
{
protected:
    using callback_t = std::function<void(T &&)>;

public:
    promise_popup(const char *title, callback_t &&bound, bool decorate = false);

protected:
    void resolve(T &&value);

private:
    callback_t bound;
};

template <class T>
promise_popup<T>::promise_popup(const char *title, callback_t &&bound, const bool decorate)
    : popup_modal(title, true, decorate), bound(std::forward<callback_t>(bound))
{
}

template <class T>
auto promise_popup<T>::resolve(T &&value) -> void
{
    close();
    if (!bound)
        return;
    std::invoke(std::forward<callback_t>(bound), std::move(value));
}
}

#endif // MYTEC_UI_HPP
