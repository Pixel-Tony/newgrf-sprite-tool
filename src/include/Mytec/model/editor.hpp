#ifndef MYTEC_EDITOR_HPP
#define MYTEC_EDITOR_HPP
#include <SFML/Graphics/Color.hpp>


namespace mytec::model
{
class editor
{
public:
    sf::Color primary = sf::Color::Black;
    sf::Color secondary;

    void swap_colors();
};
}

#endif // MYTEC_EDITOR_HPP
