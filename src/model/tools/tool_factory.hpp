#ifndef MYTEC_TOOL_FACTORY_HPP
#define MYTEC_TOOL_FACTORY_HPP

#include "tool.hpp"

namespace mytec::tool_factory
{
tool* make(tool::type _type);
}

#endif // MYTEC_TOOL_FACTORY_HPP