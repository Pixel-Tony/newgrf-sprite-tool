#ifndef MYTEC_BUTTON_HPP
#define MYTEC_BUTTON_HPP
#include "mytec/asset.hpp"
#include "mytec/vec.hpp"

#include <SFML/Graphics/Texture.hpp>

namespace mytec
{
class button
{
public:
    button(const char* _str_id, const asset& _atlas, vi2 _size);
    button(const char* _str_id, const sf::Image& _atlas, vi2 _size);

    bool render(bool _is_active) const;

private:
    const char* const str_id_;
    const sf::Texture normal_;
    const sf::Texture hover_;
    const sf::Texture active_;
};
} // namespace mytec

#endif // MYTEC_BUTTON_HPP
