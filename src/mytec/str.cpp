#include "mytec/str.hpp"

#include <cstddef>

namespace mytec::util
{
std::string ellipsis(const std::string& _input, const size_t _length)
{
    auto output = _input.substr(0, _length);
    return ellipsis(std::move(output), _length);
}

std::string ellipsis(std::string&& _input, const size_t _length)
{
    if (auto inp_len = _input.length(); inp_len > _length)
    {
        _input[inp_len - 3] = _input[inp_len - 2] = _input[inp_len - 1] = '.';
    }
    return std::move(_input);
}
} // namespace mytec::util
