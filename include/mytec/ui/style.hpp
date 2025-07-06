#ifndef MYTEC_STYLE_HPP
#define MYTEC_STYLE_HPP

#include "mytec/asset.hpp"
#include "SFML/Graphics/Color.hpp"

#include <imgui.h>

namespace mytec
{
struct style
{
    void set_active() const;

    void start_menu_bar() const;

    void init_font(const ImGuiIO& _io) const;

    static void end_menu_bar();

    sf::Color primary_;
    sf::Color secondary_;
    sf::Color tertiary_;
    sf::Color background_;
    asset font_data_;
    float font_size_ = 0;
};
} // namespace mytec

#endif // MYTEC_STYLE_HPP
