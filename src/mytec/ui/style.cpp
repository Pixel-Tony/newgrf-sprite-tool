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
    // colors[ImGuiCol_Separator] = TODO;
    // colors[ImGuiCol_SeparatorHovered] = TODO;
    // colors[ImGuiCol_SeparatorActive] = TODO;
    // colors[ImGuiCol_ResizeGrip] = TODO;            // Resize grip in lower-right and lower-left corners of windows.
    // colors[ImGuiCol_ResizeGripHovered] = TODO;
    // colors[ImGuiCol_ResizeGripActive] = TODO;
    // colors[ImGuiCol_TabHovered] = TODO;            // Tab background, when hovered
    // colors[ImGuiCol_Tab] = TODO;                   // Tab background, when tab-bar is focused & tab is unselected
    // colors[ImGuiCol_TabSelected] = TODO;           // Tab background, when tab-bar is focused & tab is selected
    // colors[ImGuiCol_TabSelectedOverline] = TODO;   // Tab horizontal overline, when tab-bar is focused & tab is
    // selected colors[ImGuiCol_TabDimmed] = TODO;             // Tab background, when tab-bar is unfocused & tab is
    // unselected colors[ImGuiCol_TabDimmedSelected] = TODO;     // Tab background, when tab-bar is unfocused & tab is
    // selected colors[ImGuiCol_TabDimmedSelectedOverline] = TODO;//..horizontal overline, when tab-bar is unfocused &
    // tab is selected colors[ImGuiCol_PlotLines] = ...; colors[ImGuiCol_PlotLinesHovered] = ...;
    // colors[ImGuiCol_PlotHistogram] = ...;
    // colors[ImGuiCol_PlotHistogramHovered] = ...;
    // colors[ImGuiCol_TableHeaderBg] = TODO;         // Table header background
    // colors[ImGuiCol_TableBorderStrong] = TODO;     // Table outer and header borders (prefer using Alpha=1.0 here)
    // colors[ImGuiCol_TableBorderLight] = TODO;      // Table inner borders (prefer using Alpha=1.0 here)
    // colors[ImGuiCol_TableRowBg] = TODO;            // Table row background (even rows)
    // colors[ImGuiCol_TableRowBgAlt] = TODO;         // Table row background (odd rows)
    // colors[ImGuiCol_TextLink] = TODO;              // Hyperlink color
    // colors[ImGuiCol_TextSelectedBg] = TODO;
    // colors[ImGuiCol_DragDropTarget] = TODO;        // Rectangle highlighting a drop target
    // colors[ImGuiCol_NavCursor] = TODO;             // Color of keyboard/gamepad navigation cursor/rectangle, when
    // visible colors[ImGuiCol_NavWindowingHighlight] = TODO; // Highlight window when using CTRL+TAB
    // colors[ImGuiCol_NavWindowingDimBg] = TODO;     // Darken/colorize entire screen behind the CTRL+TAB window list,
    // when active colors[ImGuiCol_ModalWindowDimBg] = TODO;      // Darken/colorize entire screen behind a modal
    // window, when one is active

    // colors[ImGuiCol_TabHovered] = tertiary2; // Tab background, when hovered
    // colors[ImGuiCol_Tab] = secondary2; // Tab background, when tab-bar is focused & tab is unselected
    // colors[ImGuiCol_TabSelected] = tertiary;           // Tab background, when tab-bar is focused & tab is selected
    // colors[ImGuiCol_TabSelectedOverline] = tertiary;   // Tab horizontal overline, when tab-bar is focused & tab is
    // selected

    // TODO cover other variables
}

void style::start_menu_bar() const
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {8, 8});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 4});
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1);
    ImGui::PushStyleColor(ImGuiCol_Border, secondary_);
}

void style::end_menu_bar()
{
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor();
}

void style::init_font(const ImGuiIO& _io) const
{
    auto buffer = font_data_.to_vector();
    ImFontConfig font_cfg;
    font_cfg.FontDataOwnedByAtlas = false;
    _io.Fonts->AddFontFromMemoryTTF(buffer.data(), buffer.size(), font_size_, &font_cfg);
    if (!ImGui::SFML::UpdateFontTexture())
        throw std::runtime_error{"Could not update font texture."};
}
} // namespace mytec
