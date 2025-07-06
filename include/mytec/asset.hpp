#ifndef MYTEC_ASSETS_HPP
#define MYTEC_ASSETS_HPP
#include <vector>

namespace mytec
{
struct asset
{
    [[nodiscard]] std::vector<char> to_vector() const;

    const char* data_{nullptr};
    const size_t size_{0};
};
} // namespace mytec

#endif // MYTEC_ASSETS_HPP
