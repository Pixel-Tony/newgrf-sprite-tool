#ifndef MYTEC_IMAGE_HPP
#define MYTEC_IMAGE_HPP
#include "Mytec/model/palette.hpp"
#include "Mytec/model/history_action.hpp"
#include "Mytec/tools/tool_t.hpp"
#include "Mytec/util/vec.hpp"
#include "Mytec/util/undo_redo_stack.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>


namespace mytec::model
{
namespace fs = std::filesystem;

class image final
{
    using history_t = undo_redo_stack<std::unique_ptr<history_action>>;

public:
    image(std::string &&name, v2u size, palette palette_);

    image(const fs::path &filepath, sf::Image &&content_);

    ~image() = default;

    bool put_pixel(v2u pos, sf::Color color);

    sf::Color get_pixel(v2u pos) const;

    image(image &) = delete;

    image(image &&) = default;

    image &operator=(image &) = delete;

    image &operator=(image &&) = default;

    static std::optional<image> open(const fs::path &filepath);

    bool is_valid() const noexcept;

    void close() noexcept;

    bool exists() const noexcept;

    const fs::path &get_path() const noexcept;

    const fs::path &get_name() const noexcept;

    bool dirty() const noexcept;

    float get_zoom() const noexcept;

    void handle_events(const std::unique_ptr<tools::tool_t> &tool);

    [[nodiscard]] std::pair<bool, bool> update(const std::unique_ptr<tools::tool_t> &tool, editor &editor);

    [[nodiscard]] bool try_save();

    [[nodiscard]] bool try_save(const fs::path &filepath);

    void zoom_in(v2f center) noexcept;

    void zoom_out(v2f center) noexcept;

    std::optional<v2u> get_pixel_under_mouse() const;

    bool operator==(const image &other) const noexcept;

    void commit(std::unique_ptr<history_action> &&action);

    void try_undo();

    void try_redo();

    v2f position;

private:
    void render(v2f p_min, v2f region) noexcept;

    void apply_zoom_delta(float delta, v2f center) noexcept;

    fs::path path;
    fs::path name;
    sf::Image content;
    sf::Texture texture;
    history_t history = history_t{100};
    float zoom = 1;
    bool m_valid = true;
    bool should_center = true;
    palette palette_ = palette::full_color;
    uint32_t id = id_counter++;

    uint32_t cur_history_item_id = 0;

    inline static uint32_t id_counter = 1;
};
}

#endif // MYTEC_IMAGE_HPP
