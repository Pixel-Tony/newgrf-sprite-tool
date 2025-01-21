#ifndef MYTEC_PEN_HPP
#define MYTEC_PEN_HPP
#include "Mytec/model/history_action.hpp"
#include "Mytec/tools/tool_t.hpp"
#include "Mytec/util/vec.hpp"

#include <memory>
#include <vector>


namespace mytec::tools
{
class pen final : public tool_t
{
public:
    pen();

    tool_id id() const override;

    void update(model::image &target, model::editor &editor) override;

    bool engaged() const override;

private:
    class action final : public model::history_action
    {
    public:
        explicit action(ImGuiMouseButton_ button, sf::Color color);

        ~action() override = default;

        void undo(model::image &target) const override;

        void redo(model::image &target) const override;

        void replaced(v2u position, sf::Color old_color);

        bool empty() const;

        const ImGuiMouseButton_ button;
        const sf::Color color;

    private:
        std::vector<std::pair<v2u, sf::Color>> pixels;
    };

    std::unique_ptr<action> cur_action;
};
}

#endif // MYTEC_PEN_HPP
