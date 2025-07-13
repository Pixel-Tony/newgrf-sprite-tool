#include "tool_factory.hpp"

#include "view.hpp"

#include <stdexcept>

namespace mytec::tool_factory
{
tool* make(tool::type _type)
{
    switch (_type)
    {
    case tool::view:
        return new view();
    default:
        throw std::runtime_error{"Unknown tool type"};
    }
}
} // namespace mytec::tool_factory
