#ifndef MYTEC_IMAGE_MANAGER_INL
#define MYTEC_IMAGE_MANAGER_INL

namespace mytec
{
template <typename T, typename... TArgs>
    requires(std::is_constructible_v<T, TArgs...> && std::is_base_of_v<popup_modal, T>)
void modal_stack::push(TArgs&&... _args)
{
    emplace_back(std::make_unique<T>(std::forward<TArgs>(_args)...));
}
} // namespace mytec

#endif // MYTEC_IMAGE_MANAGER_INL
