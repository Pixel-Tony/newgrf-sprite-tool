#include "Mytec/ui/style.hpp"

#include <imgui.h>


namespace
{
constexpr sf::Color operator *(const sf::Color lhs, const float v)
{
    return {
        static_cast<uint8_t>(lhs.r * v), static_cast<uint8_t>(lhs.g * v),
        static_cast<uint8_t>(lhs.b * v), static_cast<uint8_t>(lhs.a * v)
    };
}
}

namespace mytec::ui
{
style::style(sf::Color primary, sf::Color secondary, sf::Color tertiary, sf::Color quaternary, sf::Color background)
    : primary(primary), primary2(primary * 1.1f), primary3(primary * 0.9f),
      secondary(secondary), secondary2(secondary * 1.1f), secondary3(secondary * 0.9f),
      tertiary(tertiary), tertiary2(tertiary * 1.1f), tertiary3(tertiary * 0.9f),
      quaternary(quaternary), quaternary2(quaternary * 1.1f), quaternary3(quaternary * 0.9f),
      bg(background), bg2(background * 0.9f)
{
}

void style::set_active() const
{
    auto &style = ImGui::GetStyle();
    auto &colors = style.Colors;

    style.TabRounding = 0;
    style.FrameBorderSize = 0;
    style.WindowBorderSize = 0;
    style.ChildBorderSize = 0;
    style.PopupBorderSize = 1;

    // TODO
    colors[ImGuiCol_Text] = primary;
    // colors[ImGuiCol_TextDisabled] = ;
    colors[ImGuiCol_WindowBg] = bg; // Background of normal windows
    colors[ImGuiCol_ChildBg] = secondary; // Background of child windows
    colors[ImGuiCol_PopupBg] = bg; // Background of popups, menus, tooltips windows
    colors[ImGuiCol_Border] = bg;
    // colors[ImGuiCol_BorderShadow] = ;
    // colors[ImGuiCol_FrameBg] = ;               // Background of checkbox, radio button, plot, slider, text input
    // colors[ImGuiCol_FrameBgHovered] = ;
    // colors[ImGuiCol_FrameBgActive] = ;
    colors[ImGuiCol_TitleBg] = tertiary; // Title bar
    colors[ImGuiCol_TitleBgActive] = tertiary; // Title bar when focused
    // colors[ImGuiCol_TitleBgCollapsed] = ;      // Title bar when collapsed
    colors[ImGuiCol_MenuBarBg] = secondary;
    // colors[ImGuiCol_ScrollbarBg] = ;
    // colors[ImGuiCol_ScrollbarGrab] = ;
    // colors[ImGuiCol_ScrollbarGrabHovered] = ;
    // colors[ImGuiCol_ScrollbarGrabActive] = ;
    // colors[ImGuiCol_CheckMark] = ;             // Checkbox tick and RadioButton circle
    // colors[ImGuiCol_SliderGrab] = ;
    // colors[ImGuiCol_SliderGrabActive] = ;
    // colors[ImGuiCol_Button] = ;
    // colors[ImGuiCol_ButtonHovered] = ;
    // colors[ImGuiCol_ButtonActive] = ;
    colors[ImGuiCol_Header] = tertiary; // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
    // colors[ImGuiCol_HeaderHovered] = ;
    // colors[ImGuiCol_HeaderActive] = ;
    // colors[ImGuiCol_Separator] = ;
    // colors[ImGuiCol_SeparatorHovered] = ;
    // colors[ImGuiCol_SeparatorActive] = ;
    // colors[ImGuiCol_ResizeGrip] = ;            // Resize grip in lower-right and lower-left corners of windows.
    // colors[ImGuiCol_ResizeGripHovered] = ;
    // colors[ImGuiCol_ResizeGripActive] = ;
    colors[ImGuiCol_TabHovered] = tertiary; // Tab background, when hovered
    colors[ImGuiCol_Tab] = tertiary; // Tab background, when tab-bar is focused & tab is unselected
    // colors[ImGuiCol_TabSelected] = ;           // Tab background, when tab-bar is focused & tab is selected
    // colors[ImGuiCol_TabSelectedOverline] = ;   // Tab horizontal overline, when tab-bar is focused & tab is selected
    // colors[ImGuiCol_TabDimmed] = ;             // Tab background, when tab-bar is unfocused & tab is unselected
    // colors[ImGuiCol_TabDimmedSelected] = ;     // Tab background, when tab-bar is unfocused & tab is selected
    // colors[ImGuiCol_TabDimmedSelectedOverline] = ; // horizontal overline, when tab-bar is unfocused & tab is selected
    // colors[ImGuiCol_PlotLines] = ;
    // colors[ImGuiCol_PlotLinesHovered] = ;
    // colors[ImGuiCol_PlotHistogram] = ;
    // colors[ImGuiCol_PlotHistogramHovered] = ;
    // colors[ImGuiCol_TableHeaderBg] = ;         // Table header background
    // colors[ImGuiCol_TableBorderStrong] = ;     // Table outer and header borders (prefer using Alpha=1.0 here)
    // colors[ImGuiCol_TableBorderLight] = ;      // Table inner borders (prefer using Alpha=1.0 here)
    // colors[ImGuiCol_TableRowBg] = ;            // Table row background (even rows)
    // colors[ImGuiCol_TableRowBgAlt] = ;         // Table row background (odd rows)
    // colors[ImGuiCol_TextLink] = ;              // Hyperlink color
    // colors[ImGuiCol_TextSelectedBg] = ;
    // colors[ImGuiCol_DragDropTarget] = ;        // Rectangle highlighting a drop target
    // colors[ImGuiCol_NavCursor] = ;             // Color of keyboard/gamepad navigation cursor/rectangle, when visible
    // colors[ImGuiCol_NavWindowingHighlight] = ; // Highlight window when using CTRL+TAB
    // colors[ImGuiCol_NavWindowingDimBg] = ;     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
    // colors[ImGuiCol_ModalWindowDimBg] = ;      // Darken/colorize entire screen behind a modal window, when one is active
}
}
