#ifndef MYTEC_STYLE_HPP
#define MYTEC_STYLE_HPP
#include <SFML/Graphics.hpp>
#include <imgui.h>


namespace mytec::ui
{
struct style
{
    sf::Color primary, primary2, primary3;
    sf::Color secondary, secondary2, secondary3;
    sf::Color tertiary, tertiary2, tertiary3;
    sf::Color quaternary, quaternary2, quaternary3;
    sf::Color bg, bg2;

    style(sf::Color primary, sf::Color secondary, sf::Color tertiary, sf::Color quaternary, sf::Color background);

    static constexpr auto win_clear = static_cast<ImGuiWindowFlags_>(
        ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_NoMouseInputs);

    static constexpr auto win_modal = static_cast<ImGuiWindowFlags_>(
        ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_AlwaysAutoResize);

    void set_active() const;
};
}

#endif // MYTEC_STYLE_HPP
