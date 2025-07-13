#include "widgets/main_window.hpp"

#include <headers.hpp>

namespace
{
using namespace mytec;

int mytec_main(int _argc, char** _argv)
{
    QApplication app{_argc, _argv};
    QApplication::font().setPixelSize(12);
    mytec::main_window win;
    win.showMaximized();

    return QApplication::exec();
}
} // namespace

int main(int argc, char** argv) { return mytec_main(argc, argv); }