#ifndef MYTEC_UI_DIALOGS_HPP
#define MYTEC_UI_DIALOGS_HPP
#include "Mytec/model/image.hpp"
#include "Mytec/ui/promise_popup.hpp"


namespace mytec::ui
{
class create_new_image final : public promise_popup<model::image>
{
    std::string file_name;
    v2i size = {100, 100};
    model::palette palette = model::palette::full_color;

    void body() override;

public:
    explicit create_new_image(callback_t &&bound);
};


class save_closing_image final : public promise_popup<std::pair<bool, bool>>
{
    const std::string message;

    void body() override;

public:
    save_closing_image(const model::image &target, callback_t &&bound);
};
}

#endif // MYTEC_UI_DIALOGS_HPP
