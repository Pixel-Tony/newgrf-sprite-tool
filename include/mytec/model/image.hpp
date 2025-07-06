#ifndef MYTEC_IMAGE_HPP
#define MYTEC_IMAGE_HPP

#include "mytec/model/history_action.hpp"
#include "mytec/model/palette.hpp"
#include "mytec/model/tools/tool_t.hpp"
#include "mytec/util/undo_redo_stack.hpp"
#include "mytec/vec.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Texture.hpp"

#include <string>

namespace fs = std::filesystem;

namespace mytec
{
class editor;

class image final
{
    using history_t = undo_redo_stack<std::unique_ptr<history_action>>;

public:
    image(std::string&& _name, vu2 _size, palette _palette);
    image(fs::path&& _filepath, sf::Image&& _content);
    image(image&) = delete;
    image(image&&) = default;
    image& operator=(image&) = delete;
    image& operator=(image&&) = default;
    ~image() = default;

    bool operator==(const image&) const noexcept;

    [[nodiscard]] const fs::path& get_path() const noexcept;
    [[nodiscard]] std::string get_name(bool _keep_ext = true) const noexcept;
    [[nodiscard]] float get_zoom() const noexcept;
    [[nodiscard]] bool exists() const noexcept;
    [[nodiscard]] sf::Color get_pixel(vu2 _pos) const;
    [[nodiscard]] bool is_dirty() const noexcept;
    [[nodiscard]] std::optional<vu2> get_pixel_under_mouse() const;
    [[nodiscard]] static std::optional<image> open(fs::path&& _filepath);
    [[nodiscard]] bool try_save();
    [[nodiscard]] bool try_save(const fs::path& _filepath);
    [[nodiscard]] std::pair<bool, bool> update(const std::unique_ptr<tool_t>& _tool, const editor& _editor);
    bool put_pixel(vu2 _pos, sf::Color _color);
    void zoom_in(vf2 _center) noexcept;
    void zoom_out(vf2 _center) noexcept;

    void commit(std::unique_ptr<history_action>&& _action);
    void try_undo();
    void try_redo();
    [[nodiscard]] bool can_redo() const noexcept;

    vf2 position_;

private:
    void render(vf2 _p_min, vf2 _region);
    void apply_zoom_delta(float _delta, vf2 _center) noexcept;

    fs::path path_;
    std::string name_;
    sf::Image content_;
    sf::Texture texture_;
    history_t history_ = history_t{256};
    float zoom_ = 1;
    bool should_center_ = true;
    palette palette_ = palette::windows;
    uint32_t cur_history_item_id_ = 0;

public:
    uint32_t id_ = cur_id_++;

private:
    inline static uint32_t cur_id_ = 1;
};
} // namespace mytec

#endif // MYTEC_IMAGE_HPP
