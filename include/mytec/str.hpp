#ifndef MYTEC_STR_HPP
#define MYTEC_STR_HPP
#include <cstddef>
#include <string>

namespace mytec::util
{
std::string ellipsis(const std::string& _input, size_t _length);

std::string ellipsis(std::string&& _input, size_t _length);
} // namespace mytec::util

#endif // MYTEC_STR_HPP
