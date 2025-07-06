#ifndef MYTEC_APP_HPP
#define MYTEC_APP_HPP
#include "mytec/model/editor.hpp"
#include "mytec/model/image_collection.hpp"
#include "mytec/ui/components/menu_bar.hpp"
#include "mytec/ui/components/modal_stack.hpp"
#include "mytec/ui/components/palette_tab.hpp"
#include "mytec/ui/components/side_bar.hpp"
#include "mytec/ui/components/status_bar.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

namespace mytec
{
class app
{
public:
    app() = delete;
    app(const char* _title, vu2 _size, vu2 _min_size, const sf::Image& _icon, style _style, uint32_t _fps);
    app(app&) = delete;
    app(app&&) = delete;
    app& operator=(app&) = delete;
    app& operator=(app&&) = delete;
    ~app();

    int run();

private:
    void update(event _event);
    void render_fullscreen(event& _event);
    void render_canvas(event& _event);
    void handle_shortcuts(event& _event);
    void handle_event(event _event);
    bool try_save_image_as(image& _target);
    void try_save_closing_image(image* _img);
    void show_error(const char* _message);

    [[nodiscard]] bool is_user_engaged() const noexcept;

    sf::RenderWindow window_;

    style style_;
    modal_stack modals_;
    status_bar status_bar_;
    palette_tab palette_tab_;
    menu_bar menu_bar_;
    side_bar side_bar_;

    image_collection images_;
    std::unique_ptr<tool_t> tool_;
    editor editor_;
    palette cur_palette_;

    bool want_close_ = false;
};
} // namespace mytec

#endif // MYTEC_APP_HPP
