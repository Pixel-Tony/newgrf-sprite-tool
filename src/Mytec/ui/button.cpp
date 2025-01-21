#include "Mytec/ui/button.hpp"


namespace mytec::ui
{
button::button(const char *str_id, const fs::path &tex_normal, const fs::path &tex_hover, const fs::path &tex_active)
    : str_id(str_id), normal(tex_normal), hover(tex_hover), active(tex_active)
{
}

bool button::operator()(const v2f size, const bool is_active) const
{
    const v2f p_min = ImGui::GetCursorScreenPos();
    const auto click = ImGui::InvisibleButton(str_id, size);
    const auto &tex = is_active ? active : ImGui::IsItemHovered() ? hover : normal;
    ImGui::GetWindowDrawList()->AddImage(tex.getNativeHandle(), p_min, p_min + size);
    return click && !is_active;
}
}
