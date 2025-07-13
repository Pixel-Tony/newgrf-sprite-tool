#ifndef MYTEC_TOOL_HPP
#define MYTEC_TOOL_HPP

#include <qevent.h>

namespace mytec
{
class editor;
class image;

struct tool
{
    enum type
    {
        view
    };

    tool(type _type);

    virtual ~tool() = default;

    virtual bool event(QEvent* _ev, editor* _editor, image* _image);

    const type type_;
};
} // namespace mytec

#endif // MYTEC_TOOL_HPP