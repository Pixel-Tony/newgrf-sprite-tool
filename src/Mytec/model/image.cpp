#include "Mytec/model/image.hpp"
#include "Mytec/util/vec.hpp"

#include <SFML/Graphics/Image.hpp>
#include <imgui.h>
#include <format>


namespace
{
constexpr mytec::v2f zoom_bounds = {1.f, 30.f};
constexpr float edge_threshold_px = 100.f;
}

namespace mytec::model
{
image::image(std::string &&name, const v2u size, const palette palette_)
    : name(std::move(name)), content(size, sf::Color::White), texture(content), palette_(palette_)
{
}

image::image(const fs::path &filepath, sf::Image &&content_)
    : path(filepath), name(filepath.filename()), content(std::move(content_)), texture(content)
{
}

bool image::put_pixel(const v2u pos, const sf::Color color)
{
    if (content.getPixel(pos) == color)
    {
        return false;
    }
    content.setPixel(pos, color);
    texture.update(content);
    return true;
}

sf::Color image::get_pixel(const v2u pos) const { return content.getPixel(pos); }

std::optional<image> image::open(const fs::path &filepath)
{
    sf::Image result;
    if (!result.loadFromFile(filepath))
    {
        return std::nullopt;
    }
    return std::make_optional<image>(filepath, std::move(result));
}

std::pair<bool, bool> image::update(const std::unique_ptr<tools::tool_t> &tool, editor &editor)
{
    using namespace ImGui;
    assert(id != -1 && "Attempt to render invalid image");
    auto [isShown, isOpen] = std::pair{false, true};

    PushStyleVar(ImGuiStyleVar_ItemSpacing, v2f{});
    isShown = BeginTabItem(
        std::format("{}###{}", name.string().c_str(), id).c_str(),
        &isOpen,
        ImGuiTabItemFlags_NoAssumedClosure | dirty() * ImGuiTabItemFlags_UnsavedDocument
    );
    if (isShown)
    {
        handle_events(tool);
        const v2f pos = GetCursorScreenPos();
        const v2f region = GetContentRegionAvail();
        PushClipRect(pos + 1.f, pos + region - 1.f, false);
        {
            InvisibleButton("##ImageRegion", region);
            tool->update(*this, editor);
            render(pos, region);
        }
        PopClipRect();
        EndTabItem();
    }
    PopStyleVar();
    m_valid &= isOpen || dirty();
    return {isShown, isOpen};
}

const fs::path &image::get_name() const noexcept { return name; }

bool image::dirty() const noexcept
{
    const auto frame = history.peek();
    return frame != nullptr && (*frame)->id != cur_history_item_id;
}

float image::get_zoom() const noexcept { return zoom; }

void image::handle_events(const std::unique_ptr<tools::tool_t> &tool)
{
    const auto &io = ImGui::GetIO();
    if (ImGui::IsKeyPressed(ImGuiKey_Z))
    {
        if (!tool->engaged() && io.KeyCtrl)
        {
            io.KeyShift ? try_redo() : try_undo();
        }
    }
}

bool image::try_save() { return try_save(path); }

bool image::try_save(const fs::path &filepath)
{
    if (filepath.extension() != ".png")
        return false;

    if (!content.saveToFile(filepath))
        return false;

    if (const auto peek = history.peek(); peek != nullptr)
    {
        cur_history_item_id = (*peek)->id;
    }
    else
    {
        cur_history_item_id = 0;
    }
    if (filepath != path)
    {
        path = filepath;
        name = filepath.filename();
    }
    return true;
}

const fs::path &image::get_path() const noexcept { return path; }

bool image::is_valid() const noexcept { return m_valid; }

void image::close() noexcept { m_valid = false; }

bool image::exists() const noexcept { return !path.empty(); }

void image::commit(std::unique_ptr<history_action> &&action) { history.push(std::move(action)); }

void image::render(v2f p_min, const v2f region) noexcept
{
    constexpr auto fit_pad = 2.f;
    const v2f size = v2f{texture.getSize()} * zoom;
    if (region.x > size.x + fit_pad)
    {
        position.x = (region.x - size.x) / 2;
    }
    if (region.y > size.y + fit_pad)
    {
        position.y = (region.y - size.y) / 2;
    }
    if (should_center)
    {
        should_center = false;
        position = (region - size) / 2.f;
    }
    position = clamp(position, edge_threshold_px - size, region - edge_threshold_px);
    p_min += position;
    ImGui::GetWindowDrawList()->AddImage(texture.getNativeHandle(), p_min, p_min + size);
}

void image::apply_zoom_delta(const float delta, const v2f center) noexcept
{
    const auto &[min_zoom, max_zoom] = zoom_bounds;
    const auto zoom_new = std::clamp(zoom + delta, min_zoom, max_zoom);
    if (zoom_new == zoom)
        return;
    position = (position * zoom_new - center * delta) / zoom;
    zoom = zoom_new;
}

void image::zoom_in(const v2f center) noexcept { apply_zoom_delta(1.f + (zoom > 9), center); }

void image::zoom_out(const v2f center) noexcept { apply_zoom_delta(-1.f - (zoom > 10), center); }

std::optional<v2u> image::get_pixel_under_mouse() const
{
    const v2u pos = (ImGui::GetMousePos() - ImGui::GetItemRectMin() - position) / get_zoom();
    if (pos != clamp(pos, {}, content.getSize() - 1u))
        return std::nullopt;
    return pos;
}

bool image::operator==(const image &other) const noexcept { return id == other.id; }

void image::try_undo()
{
    if (const auto item = history.undo())
    {
        (*item)->undo(*this);
    }
}

void image::try_redo()
{
    if (const auto item = history.redo())
    {
        (*item)->redo(*this);
    }
}
}
