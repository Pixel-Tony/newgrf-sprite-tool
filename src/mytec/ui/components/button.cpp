#include "mytec/ui/components/button.hpp"

#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Texture.hpp"

#include <imgui.h>

namespace mytec
{
button::button(const char* _str_id, const sf::Image& _atlas, const vi2 _size)
    : str_id_(_str_id),
      normal_(_atlas, false, sf::IntRect{{0, 0}, _size}),
      hover_(_atlas, false, {{_size.x, 0}, _size}),
      active_(_atlas, false, {{_size.x * 2, 0}, _size})
{
}

button::button(const char* _str_id, const asset& _atlas, const vi2 _size)
    : button(_str_id, sf::Image{_atlas.data_, _atlas.size_}, _size)
{
}

bool button::render(const bool _is_active) const
{
    const vf2 size = vf2{normal_.getSize()};
    const vf2 p_min = ImGui::GetCursorScreenPos();
    const auto click = ImGui::InvisibleButton(str_id_, size);
    const auto& tex = _is_active ? active_ : ImGui::IsItemHovered() ? hover_ : normal_;
    ImGui::GetWindowDrawList()->AddImage(tex.getNativeHandle(), p_min, p_min + size);
    return click & !_is_active;
}
} // namespace mytec
