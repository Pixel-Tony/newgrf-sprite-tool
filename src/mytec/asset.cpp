#include "mytec/asset.hpp"

namespace mytec
{
std::vector<char> mytec::asset::to_vector() const { return std::vector<char>{data_, data_ + size_}; }
} // namespace mytec
