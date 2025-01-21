#ifndef MYTEC_TOOL_HPP
#define MYTEC_TOOL_HPP
#include "Mytec/model/editor.hpp"


namespace mytec::model
{
class image;
}

namespace mytec::tools
{
enum class tool_id
{
    view,
    pen
};

struct tool_t
{
    virtual ~tool_t() noexcept = default;

    virtual tool_id id() const = 0;

    virtual void update(model::image &target, model::editor &editor) = 0;

    virtual bool engaged() const = 0;
};
}

#endif // MYTEC_TOOL_HPP
