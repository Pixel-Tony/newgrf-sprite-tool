namespace mytec
{
pen::pen() : tool_t(tool_id::pen) {}

bool pen::is_engaged() const noexcept { return static_cast<bool>(cur_action_); }

void pen::update(image& _target, const editor& _editor)
{
    if (!cur_action_)
    {
        auto info = _editor.get_click_info();
        if (!info || !ImGui::IsItemHovered())
            return;
        cur_action_ = std::make_unique<action>(*info);
    }
    else if (!ImGui::IsMouseDown(cur_action_->initial_clicK_.button_))
    {
        if (cur_action_->empty())
            cur_action_.reset();
        else
            _target.commit(std::move(cur_action_));
        return;
    }

    const auto mouse_pos = _target.get_pixel_under_mouse();
    if (!mouse_pos)
        return;
    const auto pos = *mouse_pos;
    const auto color = _target.get_pixel(pos);
    if (_target.put_pixel(pos, cur_action_->initial_clicK_.color_))
        cur_action_->add_replaced(pos, color);
}

pen::action::action(const editor::click_info _info) : initial_clicK_(_info) {}

void pen::action::undo(image& _target) const
{
    for (const auto& [pos, old_color] : pixels_)
        _target.put_pixel(pos, old_color);
}

void pen::action::redo(image& _target) const
{
    for (const auto& pos : pixels_ | std::views::keys)
        _target.put_pixel(pos, initial_clicK_.color_);
}

void pen::action::add_replaced(const vu2 _pos, const sf::Color _old_color) { pixels_.emplace_back(_pos, _old_color); }

bool pen::action::empty() const { return pixels_.empty(); }
} // namespace mytec
