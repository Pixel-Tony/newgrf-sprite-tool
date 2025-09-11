#include "widgets/main_window.hpp"

#include <headers.hpp>

namespace
{
using namespace mytec;

int mytec_main(int _argc, char** _argv)
{
    QApplication::setOrganizationName("Tony Pixel");
    QApplication::setApplicationName("Mytec");
    QApplication::setDesktopFileName("Mytec");
    QApplication::setApplicationDisplayName("Mytec");
    QApplication app{_argc, _argv};
    mytec::main_window win;

#ifdef QT_DEBUG
    {
        const QString path = "../assets/style.qss";
        QFileSystemWatcher watcher{{path}};
        const auto cb = [&win, &path]
        {
            QFile style{path};
            style.open(QFile::ReadOnly);
            win.setStyleSheet(style.readAll());
        };
        QObject::connect(&watcher, &QFileSystemWatcher::fileChanged, cb);
        cb();
    }
#else
    {
        QFile style{":/assets/style.qss"};
        style.open(QFile::ReadOnly);
        win.setStyleSheet(style.readAll());
    }
#endif

    win.showMaximized();

    return QApplication::exec();
}
} // namespace

int main(int argc, char** argv) { return mytec_main(argc, argv); }