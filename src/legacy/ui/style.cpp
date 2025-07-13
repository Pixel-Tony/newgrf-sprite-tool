#include "mytec/ui/style.hpp"

#include "imgui.h"

#include <algorithm>
#include <imgui-SFML.h>

namespace
{
constexpr sf::Color operator*(const sf::Color _lhs, const float _v)
{
    return {static_cast<uint8_t>(std::min(255.f, _lhs.r * _v)), static_cast<uint8_t>(std::min(255.f, _lhs.g * _v)),
        static_cast<uint8_t>(std::min(255.f, _lhs.b * _v)), static_cast<uint8_t>(std::min(255.f, _lhs.a * _v))};
}
} // namespace

namespace mytec
{
void style::set_active() const
{
    static constexpr auto dimmer = 0.7f;
    static constexpr auto half_dimmer = 0.85f;
    static constexpr auto half_brighter = 1.15f;
    static constexpr auto brighter = 1.3f;

    auto& style = ImGui::GetStyle();
    auto& colors = style.Colors;

    style.TabRounding = 0;
    style.FrameBorderSize = 0;
    style.WindowBorderSize = 0;
    style.ChildBorderSize = 0;
    style.PopupBorderSize = 1;

    style.ItemSpacing = style.WindowPadding = {};

    style.DisabledAlpha = dimmer;
    colors[ImGuiCol_Text] = primary_;
    colors[ImGuiCol_TextDisabled] = primary_ * dimmer;
    colors[ImGuiCol_WindowBg] = background_;
    colors[ImGuiCol_ChildBg] = secondary_;
    colors[ImGuiCol_PopupBg] = background_;
    colors[ImGuiCol_Border] = background_;
    colors[ImGuiCol_BorderShadow] = background_ * dimmer;
    colors[ImGuiCol_FrameBg] = secondary_ * dimmer;
    colors[ImGuiCol_FrameBgHovered] = secondary_ * brighter;
    colors[ImGuiCol_FrameBgActive] = secondary_;
    colors[ImGuiCol_TitleBg] = tertiary_ * half_dimmer;
    colors[ImGuiCol_TitleBgActive] = tertiary_;
    colors[ImGuiCol_TitleBgCollapsed] = tertiary_ * dimmer;
    colors[ImGuiCol_MenuBarBg] = secondary_;
    colors[ImGuiCol_ScrollbarBg] = sf::Color::Transparent;
    colors[ImGuiCol_ScrollbarGrab] = secondary_ * half_dimmer;
    colors[ImGuiCol_ScrollbarGrabHovered] = secondary_;
    colors[ImGuiCol_ScrollbarGrabActive] = secondary_ * brighter;
    // colors[ImGuiCol_CheckMark] = ...; // Checkbox tick and RadioButton circle
    colors[ImGuiCol_SliderGrab] = tertiary_;
    colors[ImGuiCol_SliderGrabActive] = tertiary_ * half_brighter;
    colors[ImGuiCol_Button] = tertiary_ * half_dimmer;
    colors[ImGuiCol_ButtonHovered] = tertiary_ * half_brighter;
    colors[ImGuiCol_ButtonActive] = tertiary_;
    colors[ImGuiCol_Header] = secondary_;
    colors[ImGuiCol_HeaderHovered] = secondary_ * brighter * half_brighter;
    colors[ImGuiCol_HeaderActive] = secondary_ * brighter * half_brighter;
}
} // namespace mytec
