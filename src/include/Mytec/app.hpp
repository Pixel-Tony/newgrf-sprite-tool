#ifndef MYTEC_APP_HPP
#define MYTEC_APP_HPP
#include "Mytec/model/image_manager.hpp"
#include "Mytec/ui/style.hpp"

#include <SFML/Graphics/RenderWindow.hpp>


namespace mytec
{
using namespace ui;

class app
{
public:
    app() = delete;

    app(const char *title, v2u size, v2u min_size, sf::Image &&icon, style &&style);

    ~app();

    app(app &) = delete;

    app(app &&) = delete;

    app &operator=(app &) = delete;

    app &operator=(app &&) = delete;

    void run();

private:
    void process_events();

    void render_menu();

    void render();

    sf::RenderWindow window;
    model::image_manager images;
    style style_;
    bool show_demo = false;
    bool want_close = false;
};
}

#endif // MYTEC_APP_HPP
