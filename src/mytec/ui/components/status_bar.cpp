#include "mytec/ui/components/status_bar.hpp"

#include "mytec/model/image.hpp"
#include "mytec/str.hpp"
#include "mytec/ui/event.hpp"
#include "mytec/ui/misc.hpp"
#include "mytec/ui/style.hpp"

#include <format>

namespace mytec
{
status_bar::status_bar(const style& _style) : style_(_style), height_(style_.font_size_ + 2 * pad.y) {}

void status_bar::update(const event& _event) const
{
    if (auto* const img = _event.image_; ImGui::BeginChild("##StatusBar", {0, height_}) && img)
    {
        const auto zoom_v = static_cast<int32_t>(100 * img->get_zoom());
        move_cursor_screen_pos(status_bar::pad);
        auto info = std::format("{}{} @  {}%", util::ellipsis(img->get_name(), 40), " *"[img->is_dirty()], zoom_v);
        ImGui::TextUnformatted(info.c_str());
    }
    ImGui::EndChild();
}
} // namespace mytec