#ifndef MYTEC_FILL_HPP
#define MYTEC_FILL_HPP

#include "mytec/model/tools/tool_t.hpp"

namespace mytec
{
class fill final : public tool_t
{
public:
    fill();

    [[nodiscard]] bool is_engaged() const noexcept override;
    void update(image& _target, const editor& _editor) override;
};

} // namespace mytec

#endif