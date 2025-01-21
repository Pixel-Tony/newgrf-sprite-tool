#ifndef MYTEC_BUTTON_HPP
#define MYTEC_BUTTON_HPP
#include <SFML/Graphics/Texture.hpp>

#include "Mytec/util/vec.hpp"


namespace mytec::ui
{
namespace fs = std::filesystem;

class button
{
public:
    button(const char *str_id, const fs::path &tex_normal, const fs::path &tex_hover, const fs::path &tex_active);

    bool operator ()(v2f size, bool is_active) const;

private:
    const char *const str_id;
    const sf::Texture normal;
    const sf::Texture hover;
    const sf::Texture active;
};
}


#endif // MYTEC_BUTTON_HPP
