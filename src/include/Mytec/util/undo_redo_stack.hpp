#ifndef MYTEC_UNDO_REDO_STACK_HPP
#define MYTEC_UNDO_REDO_STACK_HPP
#include <vector>
#include <limits>


namespace mytec
{
template <class T>
class undo_redo_stack
{
public:
    explicit undo_redo_stack(size_t capacity);

    const T *peek() const;

    const T *undo();

    const T *redo();

    void clear();

    void push(T &&entry);

private:
    bool can_redo() const;

    bool is_empty() const;

    size_t next(size_t i) const;

    size_t prev(size_t i) const;

    size_t i_head = i_empty;
    size_t i_last = i_empty;
    size_t i_start = 0;
    size_t size;
    std::vector<T> stack;
    static constexpr size_t i_empty = std::numeric_limits<size_t>::max();
};


template <class T>
undo_redo_stack<T>::undo_redo_stack(const size_t capacity): size(capacity), stack(size)
{
}

template <class T>
const T *undo_redo_stack<T>::peek() const { return is_empty() ? nullptr : stack.data() + i_head; }

template <class T>
const T *undo_redo_stack<T>::redo() { return can_redo() ? stack.data() + (i_head = next(i_head)) : nullptr; }

template <class T>
void undo_redo_stack<T>::clear()
{
    stack.clear();
    i_head = i_empty;
    i_start = 0;
    i_last = i_empty;
}

template <class T>
void undo_redo_stack<T>::push(T &&entry)
{
    if (is_empty())
    {
        i_head = i_start;
    }
    else
    {
        i_head = next(i_head);
        if (i_start == i_head)
        {
            i_start = next(i_start);
        }
    }
    stack[i_head] = std::forward<T>(entry);
    i_last = i_head;
}

template <class T>
const T *undo_redo_stack<T>::undo()
{
    if (is_empty())
        return nullptr;

    const size_t i = i_head;
    i_head = i_head == i_start ? i_empty : prev(i_head);
    return stack.data() + i;
}

template <class T>
bool undo_redo_stack<T>::can_redo() const
{
    return i_last != i_head;
}

template <class T>
bool undo_redo_stack<T>::is_empty() const
{
    return i_head == i_empty;
}

template <class T>
size_t undo_redo_stack<T>::next(const size_t i) const
{
    return (i + 1) % size;
}

template <class T>
size_t undo_redo_stack<T>::prev(const size_t i) const
{
    return (i + size - 1) % size;
}
}

#endif // MYTEC_UNDO_REDO_STACK_HPP
