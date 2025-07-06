#ifndef MYTEC_TOOLS_HPP
#define MYTEC_TOOLS_HPP

#include "mytec/model/tools/tool_t.hpp"
#include "mytec/vec.hpp"

#include <optional>

namespace mytec
{
class image;

class view final : public tool_t
{
public:
    view();

    [[nodiscard]] bool is_engaged() const noexcept override;
    void update(image& _image, const editor& _editor) override;

private:
    std::optional<vf2> drag_start_delta_;
};
} // namespace mytec
#endif