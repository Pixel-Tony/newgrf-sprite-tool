#ifndef MYTEC_IMAGE_HPP
#define MYTEC_IMAGE_HPP
#include "palette.hpp"

#include <qimage.h>

namespace mytec
{
class image : public QImage
{
    Q_GADGET

public:
    image(QSize _size, palette::type _type, QImage::Format = QImage::Format_RGBA8888);

    [[nodiscard]] const palette* get_palette() const noexcept;

private:
    const palette* palette_;
};
} // namespace mytec

#endif // MYTEC_IMAGE_HPP

// namespace mytec
// {
// class image final
// {
// public:
//     bool operator==(const image&) const noexcept;
//     [[nodiscard]] const fs::path& get_path() const noexcept;
//     [[nodiscard]] std::string get_name(bool _keep_ext = true) const noexcept;
//     [[nodiscard]] bool exists() const noexcept;
//     [[nodiscard]] sf::Color get_pixel(vu2 _pos) const;
//     [[nodiscard]] bool is_dirty() const noexcept;
//     [[nodiscard]] std::optional<vu2> get_pixel_under_mouse() const;
//     [[nodiscard]] static std::optional<image> open(fs::path&& _filepath);
//     [[nodiscard]] bool try_save();
//     [[nodiscard]] bool try_save(const fs::path& _filepath);
//     [[nodiscard]] std::pair<bool, bool> update(const std::unique_ptr<tool_t>& _tool, const editor& _editor);
//     bool put_pixel(vu2 _pos, sf::Color _color);
//     void zoom_in(vf2 _center) noexcept;
//     void zoom_out(vf2 _center) noexcept;

//     // void commit(std::unique_ptr<history_action>&& _action);
//     void try_undo();
//     void try_redo();
//     [[nodiscard]] bool can_redo() const noexcept;

// private:
//     fs::path path_;
//     uint32_t cur_history_item_id_ = 0;
// };
// } // namespace mytec