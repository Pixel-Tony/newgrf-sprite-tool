#include "mytec/file_dialog.hpp"

#include "tinyfiledialogs.h"

#include <array>
#include <string>

namespace
{
const std::array<const char*, 1> patterns = {"*.png"};
const char* pat_description = "Sprite files, *.png";
fs::path last_location;

std::optional<fs::path> maybe_save(const char* _path)
{
    if (!_path)
        return std::nullopt;
    fs::path result = _path;
    last_location = result.parent_path();
    return result;
}
} // namespace

namespace mytec
{
std::optional<fs::path> file_dialog::save(const std::optional<std::string>& _filename)
{
    const auto location = last_location / _filename.value_or("");
    return maybe_save(
        tinyfd_saveFileDialog("Save file...", location.c_str(), patterns.size(), patterns.data(), pat_description));
}

std::optional<fs::path> file_dialog::open()
{
    return maybe_save(tinyfd_openFileDialog(
        "Open file...", last_location.c_str(), patterns.size(), patterns.data(), pat_description, false));
}

} // namespace mytec