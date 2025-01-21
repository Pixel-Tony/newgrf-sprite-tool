#ifndef WIN32
#error "File dialogs are not currently implemented for platforms other than Win32"
#endif
#include "Mytec/util/file_dialog.hpp"

#include <nfd.hpp>


namespace
{
constexpr std::initializer_list<nfdnfilteritem_t> filter{{L"Sprite files", L"png"}};
constexpr int32_t filter_size = static_cast<int32_t>(filter.size());
}


namespace mytec::util
{
file_dialog::file_dialog(const sf::WindowHandle &handle) : handle(handle)
{
}

fs::path file_dialog::try_open() const
{
    nfdnchar_t *out;
    const nfdresult_t result = NFD::OpenDialog(
        out,
        filter.begin(),
        filter_size,
        nullptr,
        {NFD_WINDOW_HANDLE_TYPE_WINDOWS, handle});

    if (result == NFD_OKAY)
        return out;
    return {};
}

fs::path file_dialog::try_save(const fs::path * const name) const
{
    nfdnchar_t *out;
    const nfdresult_t result = NFD::SaveDialog(
        out,
        filter.begin(),
        filter_size,
        nullptr,
        name ? name->c_str() : nullptr,
        {NFD_WINDOW_HANDLE_TYPE_WINDOWS, handle});

    if (result == NFD_OKAY)
        return out;
    return {};
}
}
