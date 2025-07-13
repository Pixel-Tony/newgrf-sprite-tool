#include "str.hpp"

namespace mytec
{
QString ellipsis(const QString& _input, const qsizetype _length)
{
    return _input.length() <= _length ? _input : _input.sliced(0, _length).replace(_length - 3, 3, "...");
}
} // namespace mytec
