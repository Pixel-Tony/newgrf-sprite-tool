#include "Mytec/tools/pen.hpp"
#include "Mytec/model/image.hpp"
#include "Mytec/model/editor.hpp"

#include <optional>
#include <ranges>


namespace mytec::tools
{
pen::pen(): tool_t()
{
}

tool_id pen::id() const { return tool_id::pen; }

bool pen::engaged() const { return cur_action != nullptr; }

void pen::update(model::image &target, model::editor &editor)
{
    using info_t = std::optional<std::pair<ImGuiMouseButton_, sf::Color>>;
    const info_t info =
        ImGui::IsMouseClicked(ImGuiMouseButton_Left)
            ? info_t{{ImGuiMouseButton_Left, editor.primary}}
            : ImGui::IsMouseClicked(ImGuiMouseButton_Right)
                  ? info_t{{ImGuiMouseButton_Right, editor.secondary}}
                  : std::nullopt;

    if (cur_action)
    {
        if (!ImGui::IsMouseDown(cur_action->button))
        {
            if (cur_action->empty())
            {
                cur_action.reset();
            }
            else
            {
                target.commit(std::move(cur_action));
            }
            return;
        }
        if (const auto mouse_pos = target.get_pixel_under_mouse(); mouse_pos)
        {
            // std::cout << mouse_pos->x << ", " << mouse_pos->y << "\n";
            auto &pos = *mouse_pos;
            const auto color = target.get_pixel(pos);
            if (target.put_pixel(pos, cur_action->color))
            {
                cur_action->replaced(pos, color);
            }
        }
        return;
    }
    if (!info)
    {
        return;
    }
    const auto &[button, color] = *info;
    cur_action = std::make_unique<action>(button, color);
}


pen::action::action(const ImGuiMouseButton_ button, const sf::Color color): button(button), color(color)
{
}

void pen::action::undo(model::image &target) const
{
    for (const auto &[pos, old_color]: pixels)
        target.put_pixel(pos, old_color);
}

void pen::action::redo(model::image &target) const
{
    for (const auto &pos: pixels | std::views::keys)
        target.put_pixel(pos, color);
}


void pen::action::replaced(const v2u position, const sf::Color old_color)
{
    pixels.emplace_back(position, old_color);
}

bool pen::action::empty() const { return pixels.empty(); }
}
