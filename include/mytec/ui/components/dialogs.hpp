#ifndef MYTEC_UI_DIALOGS_HPP
#define MYTEC_UI_DIALOGS_HPP

#include "mytec/model/image.hpp"
#include "mytec/ui/promise_popup.hpp"

namespace mytec
{
class message_box final : public popup_modal
{
public:
    message_box(const char* _title, const char* _message);

private:
    void body() override;

    const char* message_;
};

class create_new_image final : public promise_popup<image>
{
public:
    explicit create_new_image(callback_t&& _bound);

private:
    void body() override;

    [[nodiscard]] const char* validate() const noexcept;

    std::string file_name_;
    vi2 size_ = {100, 100};
    palette palette_ = palette::dos;
};

class save_closing_image final : public promise_popup<bool, bool>
{
public:
    save_closing_image(const image& _target, callback_t&& _bound);

private:
    void body() override;

    const std::string message_;
};
} // namespace mytec

#endif // MYTEC_UI_DIALOGS_HPP
