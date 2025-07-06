#ifndef MYTEC_EDITOR_HPP
#define MYTEC_EDITOR_HPP

#include "SFML/Graphics/Color.hpp"
#include "imgui.h"

#include <optional>

namespace mytec
{
class editor
{
public:
    struct click_info
    {
        ImGuiMouseButton_ button_{};
        sf::Color color_;
    };

    [[nodiscard]] std::optional<click_info> get_click_info() const;

    void swap_colors();

    sf::Color primary_ = sf::Color::White;
    sf::Color secondary_ = sf::Color::Blue;
};
} // namespace mytec

#endif // MYTEC_EDITOR_HPP
