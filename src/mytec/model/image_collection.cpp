#include "mytec/model/image_collection.hpp"

#include "mytec/model/image.hpp"

namespace mytec
{
std::vector<image>::const_iterator image_collection::find_if_path(const fs::path& _filepath) const noexcept
{
    return std::ranges::find_if(*this,
        [&](const image& _other)
        {
            const fs::path& rhs = _other.get_path();
            return !rhs.empty() && 0 == _filepath.compare(rhs);
        });
}

void image_collection::on_exit() {
    while (size() > 0)
    {
        if (const auto& last = back(); last.is_dirty())
            return;
        pop_back();
    }
}
} // namespace mytec
