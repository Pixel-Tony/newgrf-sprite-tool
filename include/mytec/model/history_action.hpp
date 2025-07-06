#ifndef MYTEC_HISTORY_ACTION_HPP
#define MYTEC_HISTORY_ACTION_HPP

namespace mytec
{
class image;

class history_action
{
public:
    virtual ~history_action() = default;

    virtual void undo(image& _image) const = 0;
    virtual void redo(image& _image) const = 0;

    const uint32_t id_ = cur_id_++;

private:
    inline static uint32_t cur_id_ = 1;
};
} // namespace mytec

#endif // MYTEC_HISTORY_ACTION_HPP
