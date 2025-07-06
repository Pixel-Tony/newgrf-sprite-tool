#ifndef MYTEC_FILE_DIALOG_HPP
#define MYTEC_FILE_DIALOG_HPP

#include <filesystem>
#include <optional>

namespace fs = std::filesystem;

namespace mytec::file_dialog
{
std::optional<fs::path> open();
std::optional<fs::path> save(const std::optional<std::string>& _filename = std::nullopt);
} // namespace mytec::file_dialog
#endif