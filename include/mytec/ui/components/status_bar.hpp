#ifndef MYTEC_STATUS_BAR_HPP
#define MYTEC_STATUS_BAR_HPP

#include "mytec/vec.hpp"

namespace mytec
{
class style;
class event;

class status_bar
{
public:
    status_bar(const style& _style);

    void update(const event& _event) const;

    static constexpr vf2 pad = {15, 2};

private:
    const style& style_;

public:
    float height_;
};
} // namespace mytec

#endif // MYTEC_STATUS_BAR_HPP