#ifndef MYTEC_VIEWTOOL_HPP
#define MYTEC_VIEWTOOL_HPP
#include "Mytec/model/editor.hpp"
#include "Mytec/tools/tool_t.hpp"
#include "Mytec/util/vec.hpp"


namespace mytec::tools
{
class view final : public tool_t
{
public:
    view();

    tool_id id() const override;

    void update(model::image &image, model::editor &editor) override;

    bool engaged() const override;

private:
    v2f drag_start_pos;
};
}

#endif // MYTEC_VIEWTOOL_HPP
