#ifndef MYTEC_PEN_HPP
#define MYTEC_PEN_HPP

#include "mytec/model/editor.hpp"
#include "mytec/model/history_action.hpp"
#include "mytec/model/tools/tool_t.hpp"
#include "mytec/vec.hpp"
#include "SFML/Graphics/Color.hpp"

#include <memory>
#include <vector>

namespace mytec
{
class pen final : public tool_t
{
public:
    pen();

    [[nodiscard]] bool is_engaged() const noexcept override;
    void update(image& _target, const editor& _editor) override;

private:
    class action final : public history_action
    {
    public:
        explicit action(editor::click_info _info);
        ~action() override = default;

        void undo(image& _target) const override;
        void redo(image& _target) const override;
        void add_replaced(vu2 _position, sf::Color _old_color);
        [[nodiscard]] bool empty() const;

        const editor::click_info initial_clicK_;

    private:
        std::vector<std::pair<vu2, sf::Color>> pixels_;
    };

    std::unique_ptr<action> cur_action_;
};
} // namespace mytec
#endif