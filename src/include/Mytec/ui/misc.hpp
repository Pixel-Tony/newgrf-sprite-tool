#ifndef MYTEC_MISC_HPP
#define MYTEC_MISC_HPP
#include "Mytec/util/vec.hpp"


namespace mytec::ui
{
void move_cursor_screen_pos(v2f delta);

template <class... Components>
void justify_between(auto width, Components &&... components)
{
    constexpr auto count = sizeof...(Components);
    if (ImGui::BeginTable(
        "##Controls", 3, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX))
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        const v2f space = {std::max(ImGui::GetContentRegionAvail().x - width, 0.f), 0};
        float i = 0;
        auto step = [&]
        {
            if (i) ImGui::TableNextColumn();
            move_cursor_screen_pos(space * (i / (count - 1)));
            return i + 1;
        };
        std::apply([&](auto &&... arg) { ((i = step(), arg()), ...); }, std::forward_as_tuple(components...));
        ImGui::EndTable();
    }
}
}

#endif // MYTEC_MISC_HPP
