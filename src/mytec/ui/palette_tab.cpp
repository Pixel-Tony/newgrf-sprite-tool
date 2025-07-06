#include "mytec/ui/components/palette_tab.hpp"

#include "mytec/asset.hpp"
#include "mytec/vec.hpp"
#include "imgui.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <imgui-SFML.h>

namespace
{
std::vector<sf::Color> make_palette(const mytec::asset& _data)
{
    std::vector<sf::Color> res;
    const auto* ptr = _data.data_;
    for (auto i = 0; i < 256 * 3; i += 3)
        res.emplace_back(ptr[i], ptr[i + 1], ptr[i + 2]);
    return res;
}

std::vector<sf::Color> palette_dos, palette_win;
} // namespace

namespace mytec::assets
{
extern asset palette_dos, palette_win;
}

namespace mytec
{
palette_tab::palette_tab(const style& _style) : style_(_style)
{
    if (!palette_dos.empty())
        return;
    palette_dos = make_palette(assets::palette_dos);
    palette_win = make_palette(assets::palette_win);
}

void palette_tab::update()
{
    if (!is_open_)
        return;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
    ImGui::PushStyleColor(ImGuiCol_Border, style_.secondary_);
    ImGui::SetNextWindowPos(ImGui::GetMousePos(), ImGuiCond_Appearing);
    // SetNextWindowPos({0, GetIO().DisplaySize.y - status_bar::height});
    if (ImGui::Begin("Palette", &is_open_, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static constexpr auto size = 16;
        static constexpr auto gap = 1;
        static auto width = 10;
        static auto height = 10;
        static constexpr auto pad = 3;

        auto redo = [&]()
        {
            sf::Image result{vu2{vi2{width, height} * 16 + gap * 15 + pad * 2}, sf::Color{40, 40, 40}};
            for (int32_t y = 0; y < size; ++y)
                for (int32_t dy = 0; dy < height; ++dy)
                    for (int32_t x = 0; x < size; ++x)
                        for (int32_t dx = 0; dx < width; ++dx)
                            result.setPixel(vu2{vi2{x, y} * (vi2{width, height} + gap) + vi2{dx, dy} + pad},
                                palette_dos[y * size + x]);
            return result;
        };

        static sf::Image img{redo()};
        static sf::Texture texture{img};

        if (const auto redone = ImGui::SliderInt("Width", &width, 10, 20);
            redone | ImGui::SliderInt("Height", &height, 10, 20))
            texture = sf::Texture{img = redo()};
        ImGui::Image(texture);
    }
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}
} // namespace mytec
