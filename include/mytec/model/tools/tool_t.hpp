#ifndef MYTEC_TOOL_T_HPP
#define MYTEC_TOOL_T_HPP

namespace mytec
{
class image;
class editor;

enum class tool_id
{
    none = 0,
    view,
    pen,
    fill
};

class tool_t
{
public:
    virtual ~tool_t() = default;

    const tool_id id_;
    [[nodiscard]] virtual bool is_engaged() const noexcept = 0;
    virtual void update(image& _target, const editor& _editor) = 0;

protected:
    explicit tool_t(tool_id _id);
};
} // namespace mytec
#endif