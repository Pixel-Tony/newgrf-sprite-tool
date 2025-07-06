#ifndef MYTEC_UNDO_REDO_STACK_HPP
#define MYTEC_UNDO_REDO_STACK_HPP
#include <vector>

namespace mytec
{
template <class T>
class undo_redo_stack
{
public:
    explicit undo_redo_stack(size_t _capacity);

    [[nodiscard]] const T* peek() const;
    [[nodiscard]] const T* undo();
    [[nodiscard]] const T* redo();
    void clear();
    void push(T&& _entry);

    [[nodiscard]] bool can_redo() const;
    [[nodiscard]] bool is_empty() const;

private:
    [[nodiscard]] size_t next(size_t _i) const;
    [[nodiscard]] size_t prev(size_t _i) const;

    size_t i_head_ = i_empty;
    size_t i_last_ = i_empty;
    size_t i_start_ = 0;
    size_t size_;
    std::vector<T> stack_;
    static constexpr size_t i_empty = -1;
};
} // namespace mytec

#include "undo_redo_stack.inl"

#endif // MYTEC_UNDO_REDO_STACK_HPP
