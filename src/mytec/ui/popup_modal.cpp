#include "mytec/ui/popup_modal.hpp"

#include "mytec/vec.hpp"

#include <imgui.h>

namespace mytec
{
void popup_modal::close() { is_open_ = false; }

popup_modal::popup_modal(const char* _title, bool _cross, bool _close_on_quit)
    : title_(_title),
      has_cross_(_cross),
      close_on_quit_(_close_on_quit)
{
}

bool popup_modal::is_open() const noexcept { return is_open_; }

bool popup_modal::update()
{
    if (should_open_)
    {
        is_open_ = true;
        should_open_ = false;
        ImGui::OpenPopup(title_);
    }
    else if (!is_open_)
        return false;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {20, 12});
    ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize * 0.5f, ImGuiCond_Appearing, vf2{1, 1} * 0.5f);
    if (ImGui::BeginPopupModal(title_, has_cross_ ? (&is_open_) : nullptr,
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
    {
        body();
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    return is_open_;
}
} // namespace mytec
