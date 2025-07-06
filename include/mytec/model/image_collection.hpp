#ifndef MYTEC_IMAGE_MANAGER_HPP
#define MYTEC_IMAGE_MANAGER_HPP

#include "mytec/model/image.hpp"
namespace mytec
{
namespace fs = std::filesystem;

class image_collection : public std::vector<image>
{
public:
    [[nodiscard]] const_iterator find_if_path(const fs::path& _filepath) const noexcept;
    void on_exit();
};
} // namespace mytec

#endif // MYTEC_IMAGE_MANAGER_HPP
