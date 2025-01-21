#ifndef MYTEC_FILE_DIALOG_HPP
#define MYTEC_FILE_DIALOG_HPP
#include <SFML/Window/WindowHandle.hpp>
#include <filesystem>


namespace mytec::util
{
namespace fs = std::filesystem;

class file_dialog
{
    const sf::WindowHandle handle;

public:
    explicit file_dialog(const sf::WindowHandle &);

    fs::path try_open() const;

    fs::path try_save(const fs::path *name = nullptr) const;
};
}


#endif // MYTEC_FILE_DIALOG_HPP
