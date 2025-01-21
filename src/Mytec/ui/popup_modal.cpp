#include "Mytec/ui/popup_modal.hpp"
#include "Mytec/util/vec.hpp"
#include "Mytec/ui/style.hpp"


namespace mytec::ui
{
void popup_modal::close() { is_open_ = false; }

popup_modal::popup_modal(const char *title, const bool open, const bool decorate)
    : title(title), should_open(open), decorate(decorate)
{
}

bool popup_modal::is_open() const noexcept { return is_open_; }

void popup_modal::show() { should_open = true; }

bool popup_modal::update()
{
    if (should_open)
    {
        is_open_ = true;
        should_open = false;
        ImGui::OpenPopup(title);
    }
    else if (!is_open_)
    {
        return false;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {20, 12});
    ImGui::SetNextWindowPos(ImGui::GetIO().DisplaySize * 0.5f, ImGuiCond_Appearing, v2f{1, 1} * 0.5f);
    if (ImGui::BeginPopupModal(title, decorate ? &is_open_ : nullptr, style::win_modal))
    {
        body();
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    return is_open_;
}
}
