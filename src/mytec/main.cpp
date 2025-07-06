#include "mytec/app.hpp"
#include "mytec/asset.hpp"

namespace mytec::assets
{
extern const asset main_icon;
extern const asset font_data;
} // namespace mytec::assets

namespace
{
using namespace mytec;

int mytec_main()
{
    auto main_icon = sf::Image{assets::main_icon.data_, assets::main_icon.size_};
    mytec::style style{.primary_ = {0xFA, 0xF8, 0xE8}, // pale yellow
        .secondary_ = {0x3E, 0x43, 0x48},              // dark grayish blue
        .tertiary_ = {0x46, 0x8C, 0x4D},               // dark lime green
        .background_ = {0x1D, 0x1D, 0x26},             // very dark gray
        .font_data_ = assets::font_data,
        .font_size_ = 15};

    mytec::app app{"Mytec", {1280u, 720u}, {800u, 600u}, main_icon, style, 120u};
    return app.run();
}
} // namespace

#ifdef MYTEC_WIN32_EXECUTABLE
int WinMain() { return mytec_main(); }
#else
int main() { return mytec_main(); }
#endif