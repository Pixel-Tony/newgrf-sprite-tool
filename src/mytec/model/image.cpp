#include "mytec/model/image.hpp"

#include <imgui.h>

namespace mytec
{
image::image(std::string&& _name, const vu2 _size, const palette _palette)
    : name_(std::move(_name)),
      content_(_size, sf::Color::White),
      texture_(content_),
      palette_(_palette)
{
}

image::image(fs::path&& _filepath, sf::Image&& _content)
    : path_(std::move(_filepath)),
      name_(path_.filename()),
      content_(std::move(_content)),
      texture_(content_)
{
}

bool image::operator==(const image& _other) const noexcept { return id_ == _other.id_; }

const fs::path& image::get_path() const noexcept { return path_; }

std::string image::get_name(bool _keep_ext) const noexcept {
    if (_keep_ext || !name_.ends_with(".png"))
        return name_;
    return name_.substr(0, name_.length() - 4);
}

float image::get_zoom() const noexcept { return zoom_; }

bool image::exists() const noexcept { return !path_.empty(); }

sf::Color image::get_pixel(const vu2 _pos) const { return content_.getPixel(_pos); }

bool image::is_dirty() const noexcept
{
    const auto* const frame = history_.peek();
    return cur_history_item_id_ != (frame ? (*frame)->id_ : 0u);
}

bool image::can_redo() const noexcept { return history_.can_redo(); }

std::optional<vu2> image::get_pixel_under_mouse() const
{
    const vu2 pos = (ImGui::GetMousePos() - ImGui::GetItemRectMin() - position_) / get_zoom();
    if (pos != clamp(pos, {}, content_.getSize() - 1u))
        return std::nullopt;
    return pos;
}

std::optional<image> image::open(fs::path&& _filepath)
{
    sf::Image result;
    if (!result.loadFromFile(_filepath))
        return std::nullopt;
    return std::make_optional<image>(std::move(_filepath), std::move(result));
}

bool image::try_save() { return try_save(path_); }

bool image::try_save(const fs::path& _filepath)
{
    if (_filepath.extension() != ".png" || !content_.saveToFile(_filepath))
        return false;

    const auto* const peek = history_.peek();
    cur_history_item_id_ = peek ? (*peek)->id_ : 0;
    if (_filepath != path_)
    {
        path_ = _filepath;
        name_ = path_.filename();
    }
    return true;
}

std::pair<bool, bool> image::update(const std::unique_ptr<tool_t>& _tool, const editor& _editor)
{
    auto is_shown = false;
    auto is_kept_open = true;

    const auto flags = ImGuiTabItemFlags_NoAssumedClosure | (is_dirty() * ImGuiTabItemFlags_UnsavedDocument);
    const auto str_id = name_ + "###" + std::to_string(id_);
    if ((is_shown = ImGui::BeginTabItem(str_id.c_str(), &is_kept_open, flags)))
    {
        const vf2 pos = ImGui::GetCursorScreenPos();
        const vf2 region = ImGui::GetContentRegionAvail();
        ImGui::PushClipRect(pos, pos + region, false);
        {
            render(pos, region);
            _tool->update(*this, _editor);
        }
        ImGui::PopClipRect();
        ImGui::EndTabItem();
    }
    return {is_shown, is_kept_open};
}

void image::render(vf2 _p_min, const vf2 _region)
{
    const vf2 size = vf2{texture_.getSize()} * zoom_;

    // Separately center image axes if they fit inside canvas region (excluding some padding)
    auto half_bounds = (_region - size) / 2.f;
    auto fits = vf2{half_bounds > 30.f};
    position_ = position_ * (1.f - fits) + fits * half_bounds;

    if (should_center_)
    {
        should_center_ = false;
        position_ = half_bounds;
    }

    constexpr auto edge_threshold_px = 100.f;
    position_ = clamp(position_, edge_threshold_px - size, _region - edge_threshold_px);
    _p_min += position_;
    ImGui::InvisibleButton("##ImageRegion", _region);
    ImGui::GetWindowDrawList()->AddImage(texture_.getNativeHandle(), _p_min, _p_min + size);
}

bool image::put_pixel(const vu2 _pos, const sf::Color _color)
{
    if (content_.getPixel(_pos) == _color)
        return false;
    content_.setPixel(_pos, _color);
    texture_.update(content_);
    return true;
}

void image::zoom_in(const vf2 _center) noexcept { apply_zoom_delta(1.f + (zoom_ > 9), _center); }

void image::zoom_out(const vf2 _center) noexcept { apply_zoom_delta(-1.f - (zoom_ > 10), _center); }

void image::apply_zoom_delta(const float _delta, const vf2 _center) noexcept
{
    const auto min_zoom = 1.f;
    const auto max_zoom = 30.f;
    const auto zoom_new = std::clamp(zoom_ + _delta, min_zoom, max_zoom);
    if (zoom_new == zoom_)
        return;
    position_ = (position_ * zoom_new - _center * _delta) / zoom_;
    zoom_ = zoom_new;
}

void image::commit(std::unique_ptr<history_action>&& _action) { history_.push(std::move(_action)); }

void image::try_undo()
{
    if (const auto* const item = history_.undo())
        (*item)->undo(*this);
}

void image::try_redo()
{
    if (const auto* const item = history_.redo())
        (*item)->redo(*this);
}
} // namespace mytec
