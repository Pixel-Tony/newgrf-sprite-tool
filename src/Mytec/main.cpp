#include "Mytec/app.hpp"


int main()
{
    const auto title = "Mytec v" PROJECT_VER;
    const auto icon_path = "logo-128.png";

    constexpr sf::Color primary = {0xFA, 0xF8, 0xE8},
        secondary = {0x3E, 0x43, 0x48},
        tertiary = {0x46, 0x8C, 0x4D},
        quaternary = {0x63, 0xE6, 0xE1},
        background = {0x1D, 0x1D, 0x26};

    mytec::app app{
        title,
        {1280u, 720u},
        {800u, 600u},
        sf::Image{icon_path},
        mytec::style{primary, secondary, tertiary, quaternary, background}
    };
    app.run();
    return EXIT_SUCCESS;
}

#ifdef WIN32_EXECUTABLE
int WinMain() { return Main(); }
#endif
