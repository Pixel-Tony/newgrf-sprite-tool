#ifndef MYTEC_IMAGE_MANAGER_HPP
#define MYTEC_IMAGE_MANAGER_HPP

#include "Mytec/model/image.hpp"
#include "Mytec/tools/view.hpp"
#include "Mytec/ui/message_box.hpp"
#include "Mytec/ui/dialogs.hpp"
#include "Mytec/util/file_dialog.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>


namespace mytec::model
{
namespace fs = std::filesystem;

class image_manager
{
public:
    explicit image_manager(const sf::WindowHandle &handle);

    bool try_close(bool &want_close);

    void open_file();

    bool try_resave(image &target);

    void handle_events();

    void update();

private:
    static constexpr size_t i_no_cur_image = std::numeric_limits<size_t>::max();

    bool is_image_open(const fs::path &path) const;

    std::vector<image>::const_iterator find_image_open(const fs::path &filepath) const;

    auto show_error(const char *message) -> void;

    auto render_menu_bar_entry(size_t i_cur_image) -> void;

    util::file_dialog file_dialog;
    std::unique_ptr<tools::tool_t> tool;
    std::vector<image> images;
    std::unique_ptr<ui::create_new_image> dialog_new_image;
    std::unique_ptr<ui::save_closing_image> dialog_save_closing_image;
    std::unique_ptr<ui::message_box> message_box;
    editor editor_;
};
}

#endif // MYTEC_IMAGE_MANAGER_HPP
