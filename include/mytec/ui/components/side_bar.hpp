#ifndef MYTEC_SIDE_BAR_HPP
#define MYTEC_SIDE_BAR_HPP

#include "mytec/model/tools/tool_t.hpp"
#include "mytec/ui/components/button.hpp"
#include "mytec/ui/style.hpp"
#include "mytec/vec.hpp"

namespace mytec
{
class editor;
class event;
class status_bar;

class side_bar
{
public:
    side_bar(style& _style, const status_bar& _status_bar);

    void update(event& _event, const editor& _editor, const std::unique_ptr<tool_t>& _tool);

    static constexpr auto icon_width = 24;
    static constexpr auto icon_size = vi2{icon_width, icon_width};
    static constexpr auto padding = vi2{4, 6};
    static constexpr auto width = icon_width + padding.x * 2;

    const style& style_;

private:
    void update_palette_btn(event& _event, const editor& _editor);

    std::vector<std::tuple<tool_id, button>> buttons_;
    float status_bar_height_;
};
} // namespace mytec
#endif // MYTEC_SIDE_BAR_HPP