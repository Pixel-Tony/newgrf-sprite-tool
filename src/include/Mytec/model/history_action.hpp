#ifndef MYTEC_HISTORY_ACTION_HPP
#define MYTEC_HISTORY_ACTION_HPP
#include <cstdint>


namespace mytec::model
{
class image;

class history_action
{
public:
    virtual ~history_action() = default;

    virtual void undo(image &image) const = 0;

    virtual void redo(image &image) const = 0;

    const uint32_t id = id_counter++;

private:
    inline static uint32_t id_counter = 1;
};
}

#endif // MYTEC_HISTORY_ACTION_HPP
