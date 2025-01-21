#include "Mytec/util/str.hpp"

#include <iostream>

namespace mytec::util
{
std::string ellipsis(const std::string &input, const size_t length)
{
    if (input.length() <= length)
    {
        return input;
    }
    return input.substr(0, length - 3) + "...";
}
}
