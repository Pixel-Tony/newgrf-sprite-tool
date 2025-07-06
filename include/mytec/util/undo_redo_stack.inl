#ifndef MYTEC_UNDO_REDO_STACK_INL
#define MYTEC_UNDO_REDO_STACK_INL
#include "undo_redo_stack.hpp"

namespace mytec
{
template <class T>
undo_redo_stack<T>::undo_redo_stack(const size_t _capacity) : size_(_capacity), stack_(size_) {}

template <class T>
const T* undo_redo_stack<T>::peek() const { return is_empty() ? nullptr : stack_.data() + i_head_; }

template <class T>
const T* undo_redo_stack<T>::redo() { return can_redo() ? stack_.data() + (i_head_ = next(i_head_)) : nullptr; }

template <class T>
void undo_redo_stack<T>::clear()
{
    stack_.clear();
    i_head_ = i_empty;
    i_start_ = 0;
    i_last_ = i_empty;
}

template <class T>
void undo_redo_stack<T>::push(T&& _entry)
{
    if (is_empty())
        i_head_ = i_start_;
    else
    {
        i_head_ = next(i_head_);
        if (i_start_ == i_head_)
        {
            i_start_ = next(i_start_);
        }
    }
    stack_[i_head_] = std::forward<T>(_entry);
    i_last_ = i_head_;
}

template <class T>
const T* undo_redo_stack<T>::undo()
{
    if (is_empty())
        return nullptr;

    const size_t i = i_head_;
    i_head_ = i_head_ == i_start_ ? i_empty : prev(i_head_);
    return stack_.data() + i;
}

template <class T>
bool undo_redo_stack<T>::can_redo() const { return i_last_ != i_head_; }

template <class T>
bool undo_redo_stack<T>::is_empty() const { return i_head_ == i_empty; }

template <class T>
size_t undo_redo_stack<T>::next(const size_t _i) const { return (_i + 1) % size_; }

template <class T>
size_t undo_redo_stack<T>::prev(const size_t _i) const { return (_i + size_ - 1) % size_; }
} // namespace mytec

#endif