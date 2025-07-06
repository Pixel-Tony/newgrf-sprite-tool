
#include "mytec/ui/components/menu_bar.hpp"

#include "mytec/model/image.hpp"
#include "mytec/model/tools/tool_t.hpp"
#include "mytec/ui/event.hpp"

#include <memory>

namespace mytec
{
menu_bar::menu_bar(style& _style) : style_(_style) {}

void menu_bar::update(event& _event, bool _usable, const std::unique_ptr<tool_t>& _tool) const
{
    using namespace ImGui;

    const auto *_img = _event.image_;
    style_.start_menu_bar();
    if (BeginMainMenuBar())
    {
        if (BeginMenu("File"))
        {
            BeginDisabled(!_usable);
            _event.make_on(MenuItem("New...", "Ctrl+N"), event_type::create);
            Separator();
            _event.make_on(MenuItem("Open...", "Ctrl+O"), event_type::open);
            Separator();
            _event.make_on(
                MenuItem("Save", "Ctrl+S", nullptr, _img && _img->exists() && _img->is_dirty()), event_type::save);
            _event.make_on(MenuItem("Save as...", "Ctrl+Shift+S", false, _img), event_type::save_as);
            EndDisabled();
            EndMenu();
        }
        if (BeginMenu("Edit"))
        {
            BeginDisabled(!_usable);
            _event.make_on(MenuItem("Undo", "Ctrl+Z", false, _img && _img->is_dirty()), event_type::undo);
            _event.make_on(MenuItem("Redo", "Ctrl+Shift+Z", false, _img && _img->can_redo()), event_type::redo);
            Separator();
            _event.make_on(MenuItem("Swap colors", "X"), event_type::swap_colors);
            EndDisabled();
            EndMenu();
        }
        if (BeginMenu("Selection")) // TODO
            EndMenu();
        if (BeginMenu("View"))
        {
            BeginDisabled(!_usable);
            _event.make_on(MenuItem("Zoom in", "MouseUp"), event_type::none);    // TODO
            _event.make_on(MenuItem("Zoom out", "MouseDown"), event_type::none); // TODO
            _event.make_on(MenuItem("Max zoom"), event_type::none);              // TODO
            _event.make_on(MenuItem("100%"), event_type::none);                  // TODO
            EndDisabled();
            EndMenu();
        }
        if (BeginMenu("Image")) // TODO
            EndMenu();
        if (BeginMenu("Tools"))
        {
            const auto id = _tool->id_;
            BeginDisabled(!_usable);
            _event.tool_on(MenuItem("View", "V", false, id != tool_id::view), tool_id::view);
            _event.tool_on(MenuItem("Pen", "B", false, id != tool_id::pen), tool_id::pen);
            _event.tool_on(MenuItem("Fill", "F", false, id != tool_id::fill), tool_id::fill);
            EndDisabled();
            EndMenu();
        }
        EndMainMenuBar();
    }
    style::end_menu_bar();
}
} // namespace mytec