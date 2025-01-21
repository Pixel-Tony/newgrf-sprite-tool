#ifndef MYTEC_MESSAGEBOX_HPP
#define MYTEC_MESSAGEBOX_HPP
#include "Mytec/ui/promise_popup.hpp"


namespace mytec::ui
{
class message_box final : public popup_modal
{
    const char *message;

    void body() override;

public:
    message_box(const char *title, const char *message);
};
}

#endif // MYTEC_MESSAGEBOX_HPP
