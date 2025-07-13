#include "tool.hpp"

namespace mytec
{
tool::tool(tool::type _type) : type_(_type) {}

bool tool::event(QEvent*, editor*, image*) { return false; }
} // namespace mytec