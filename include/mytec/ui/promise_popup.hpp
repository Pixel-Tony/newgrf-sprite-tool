#ifndef MYTEC_UI_HPP
#define MYTEC_UI_HPP
#include "mytec/ui/popup_modal.hpp"

#include <functional>

namespace mytec
{
template <class... Ts>
class promise_popup : public popup_modal
{
protected:
    using callback_t = std::function<void(Ts&&...)>;

public:
    promise_popup(const char* _title, callback_t&& _bound, bool _has_cross, bool _close_on_quit);

protected:
    void resolve(Ts&&... _args);

private:
    callback_t bound_;
};
} // namespace mytec

#include "promise_popup.inl"

#endif // MYTEC_UI_HPP
