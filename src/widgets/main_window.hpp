#ifndef MYTEC_MAIN_WINDOW_HPP
#define MYTEC_MAIN_WINDOW_HPP

#include "canvas.hpp"
#include "dialogs/create_file_dialog.hpp"
#include "editor.hpp"
#include "model/palette_action.hpp"
#include "widgets/palette_tab.hpp"

#include <headers.hpp>

namespace mytec
{
class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window();

protected:
    void closeEvent(QCloseEvent*) override;

private slots:
    void on_active_editor_changed(editor* _editor);
    void on_tool_active_changed(bool _engaged);

private:
    void show_message(const QString& _message, QMessageBox::Icon _icon = QMessageBox::Warning);
    void create_actions_menus();

    canvas* canv_ = nullptr;
    QLabel* status_bar_ = nullptr;
    create_file_dialog* create_fd_ = nullptr;
    QFileDialog* open_fd_ = nullptr;
    QMessageBox* message_box_ = nullptr;
    palette_tab* palette_tab_ = nullptr;

    QMenu* file_menu_ = nullptr;
    QAction* create_ = nullptr;
    QAction* open_ = nullptr;
    QAction* save_ = nullptr;
    QAction* save_as_ = nullptr;
    QAction* close_ = nullptr;
    QAction* exit_ = nullptr;

    QMenu* edit_menu_ = nullptr;
    QAction* undo_ = nullptr;
    QAction* redo_ = nullptr;
    QAction* swap_colors_ = nullptr;

    QMenu* view_menu_ = nullptr;
    QAction* zoom_in_ = nullptr;
    QAction* zoom_out_ = nullptr;
    QAction* default_zoom_ = nullptr;
    palette_action* palette_toggle_ = nullptr;

    QMenu* tool_menu_ = nullptr;

    QActionGroup* tool_group_ = nullptr;
    QAction* view_ = nullptr;
    QAction* pen_ = nullptr;
};
} // namespace mytec

#endif // MYTEC_MAIN_WINDOW_HPP
