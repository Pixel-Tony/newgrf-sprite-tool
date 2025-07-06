#include "mytec/ui/components/side_bar.hpp"

#include "mytec/model/editor.hpp"
#include "mytec/model/tools/tool_t.hpp"
#include "mytec/ui/components/button.hpp"
#include "mytec/ui/components/status_bar.hpp"
#include "mytec/ui/event.hpp"
#include "mytec/ui/style.hpp"
#include "mytec/vec.hpp"

#include <SFML/Graphics/Image.hpp>
#include <memory>
#include <vector>

namespace mytec::assets
{
extern const asset atlas_pen, atlas_view;
}

namespace mytec
{
void side_bar::update(event& _event, const editor& _editor, const std::unique_ptr<tool_t>& _tool)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, padding);
    if (ImGui::BeginChild("##SideBar", {width, -(status_bar_height_ + 1)}, ImGuiChildFlags_AlwaysUseWindowPadding))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, padding);

        const auto cur_id = _tool->id_;
        const auto is_engaged = _tool->is_engaged();
        for (const auto& [id, button] : buttons_)
            _event.tool_on(button.render(id == cur_id) && !is_engaged, id);

        update_palette_btn(_event, _editor);
        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
    ImGui::PopStyleVar();
}

side_bar::side_bar(style& _style, const status_bar& _status_bar)
    : style_(_style),
      buttons_({{tool_id::view, button{"BtnView", assets::atlas_view, icon_size}},
          {tool_id::pen, button{"BtnPen", assets::atlas_pen, icon_size}}}),
      status_bar_height_(_status_bar.height_)
{
}

void side_bar::update_palette_btn(event& _event, const editor& _editor)
{
    static const auto size = vf2{18, 18};
    static const auto icon_dims = vf2{icon_size};
    static const auto delta = icon_dims - size - 1.f + vf2{0, 5};

    auto clicked = false;
    auto* const draw_list = ImGui::GetWindowDrawList();
    const vf2 p_start = ImGui::GetCursorScreenPos();
    const auto background = ImGui::ColorConvertFloat4ToU32(style_.background_);
    for (auto [d_pos, color, id] : {std::tuple{delta, _editor.secondary_, "1"}, {vf2{1, 1}, _editor.primary_, "2"}})
    {
        vf2 pos = p_start + d_pos;
        draw_list->AddRect(pos, pos + size, background);
        draw_list->AddRectFilled(pos + 1.f, pos + size - 1.f, ImGui::ColorConvertFloat4ToU32(color));
        ImGui::SetCursorScreenPos(pos);
        clicked |= ImGui::InvisibleButton(id, size);
    }
    _event.make_on(clicked, event_type::toggle_palette_visibility);
    ImGui::SetCursorScreenPos(p_start + (icon_dims + vf2{padding} + 5.f) * vf2{0, 1});
}
} // namespace mytec