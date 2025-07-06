#include "mytec/str.hpp"
#include "mytec/ui/components/dialogs.hpp"

#include <format>
#include <imgui.h>

namespace mytec
{
save_closing_image::save_closing_image(const image& _target, callback_t&& _bound)
    : promise_popup("Unsaved file", std::forward<callback_t>(_bound), false, false),
      message_(std::format("Image \"{}\" has unsaved changes.", util::ellipsis(_target.get_name(), 15)))
{
}

void save_closing_image::body()
{
    static constexpr vf2 btn_size = {80, 0};

    ImGui::TextUnformatted(message_.c_str());
    ImGui::Dummy({0, 10});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {8, 0});
    if (ImGui::Button("Save", btn_size))
        resolve(false, true);
    ImGui::SameLine();
    if (ImGui::Button("Close", btn_size))
        resolve(false, false);
    ImGui::SameLine();
    if (ImGui::Button("Cancel", btn_size) || ImGui::IsKeyPressed(ImGuiKey_Escape))
        resolve(true, false);
    ImGui::PopStyleVar();
}
} // namespace mytec
