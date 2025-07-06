#ifndef MYTEC_MODEL_STACH_HPP
#define MYTEC_MODEL_STACH_HPP

namespace mytec
{
class popup_modal;

class modal_stack : public std::vector<std::unique_ptr<popup_modal>>
{
public:
    template <class T, class... TArgs>
        requires(std::is_constructible_v<T, TArgs...> && std::is_base_of_v<popup_modal, T>)
    void push(TArgs&&... _args);

    void update();

    void on_exit();
};
} // namespace mytec

#include "modal_stack.inl"

#endif // MYTEC_MODEL_STACH_HPP