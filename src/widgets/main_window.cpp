#include "main_window.hpp"

#include "canvas.hpp"
#include "model/image.hpp"
#include "model/palette_action.hpp"
#include "model/tools/tool.hpp"
#include "palette_tab.hpp"
#include "util/str.hpp"
#include "widgets/dialogs/create_file_dialog.hpp"
#include "widgets/editor.hpp"

namespace
{
QAction *make_tool_action(QActionGroup *_group, const QString &_name, const QKeySequence &_shortcut,
    const QString &_atlas, mytec::tool::type _type)
{
    auto *const _target = _group->addAction(_name);
    QIcon icon;
    {
        const auto atlas = QPixmap{_atlas};
        const auto h = atlas.height();
        icon.addPixmap(atlas.copy(0, 0, h, h), QIcon::Normal, QIcon::Off);
        icon.addPixmap(atlas.copy(h, 0, h, h), QIcon::Normal, QIcon::On);
    }
    _target->setIcon(icon);
    _target->setIconVisibleInMenu(false);
    _target->setShortcut(_shortcut);
    _target->setCheckable(true);
    _target->setData(QVariant::fromValue(_type));
    return _target;
}
} // namespace

namespace mytec
{
main_window::main_window()
    : canv_(new canvas),
      status_bar_(new QLabel),
      create_fd_(new create_file_dialog(this)),
      open_fd_(new QFileDialog(this, "Choose file to open", {}, "*.png")),
      message_box_(new QMessageBox(QMessageBox::Icon::Information, {}, {}, QMessageBox::StandardButton::Ok, this)),
      palette_tab_(new palette_tab(this)),
      palette_action_(new palette_action(this)),
      tool_group_(new QActionGroup(this))
{
    initialize_opengl_context_->hide();
    setWindowTitle("Mytec");
    setMinimumSize(800, 600);
    setWindowIcon(QIcon{":/assets/logo-128.png"});

    open_fd_->setNameFilter("Image Files (*.png)");
    open_fd_->setAcceptMode(QFileDialog::AcceptOpen);
    open_fd_->setModal(true);
    connect(open_fd_, &QFileDialog::accepted, canv_, [this]() { canv_->open_image(open_fd_->selectedFiles()[0]); });

    create_actions_menus();
    setCentralWidget(canv_);

    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, palette_tab_);

    connect(create_fd_, &create_file_dialog::confirmed, canv_, &canvas::create_image);
    connect(canv_, &canvas::changed, this, &main_window::on_active_editor_changed);
    connect(canv_, &canvas::failed, [this](const QString &_msg) { show_message(_msg); });
    connect(canv_, &canvas::exit_prepared, this, &main_window::close);
    statusBar()->addWidget(status_bar_);

    load_gui_state();
    canv_->bootstrap();
}

void main_window::load_gui_state()
{
    QSettings settings;
    palette_tab_->setVisible(settings.value("paletteTab/visible", false).toBool());
    open_fd_->setDirectory(settings.value("openFileDialog/dir", QString()).toString());
    canv_->load_gui_state(settings);
}

void main_window::write_gui_state()
{
    QSettings settings;
    settings.setValue("paletteTab/visible", palette_tab_->isVisible());
    settings.setValue("openFileDialog/dir", open_fd_->directory().absolutePath());
    canv_->save_gui_state(settings);
}

void main_window::closeEvent(QCloseEvent *_ev)
{
    bool will_exit = canv_->try_exit();
    if (will_exit)
        write_gui_state();
    _ev->setAccepted(will_exit);
}

void main_window::on_active_editor_changed(const editor *const _editor)
{
    const bool enabled = _editor;
    for (auto *a : QList{save_as_, close_, default_zoom_})
        a->setEnabled(enabled);
    save_->setEnabled(enabled && (!_editor->exists() || !_editor->history()->isClean()));
    zoom_out_->setEnabled(enabled && _editor->zoom() != editor::zoom_bounds.first);
    zoom_in_->setEnabled(enabled && _editor->zoom() != editor::zoom_bounds.second);
    undo_->setEnabled(enabled && _editor->history()->canUndo());
    redo_->setEnabled(enabled && _editor->history()->canRedo());

    QString status_bar_text;
    QString title_text = "%0";
    if (enabled)
    {
        const auto &img = _editor->get_image();
        const auto [w, h] = img.contents().size();
        const char dirty_asterisk = "* "[_editor->history()->isClean()];
        const auto *const palette = img.get_palette();
        status_bar_text = tr("%0%1 (%2) | %3x%4 | %5%")
                              .arg(ellipsis(_editor->name(), 50), dirty_asterisk, palette ? palette->name() : "32bpp",
                                  QString::number(w), QString::number(h),
                                  QString::number(static_cast<long>(_editor->zoom() * 100)));
        title_text = tr("%0%1 - %2").arg(_editor->name(), dirty_asterisk);
        palette_tab_->set_palette(palette ? palette->type_ : palette::none);
    }
    setWindowTitle(title_text.arg(QApplication::applicationDisplayName()));
    status_bar_->setText(status_bar_text);
}

void main_window::show_message(const QString &_message, QMessageBox::Icon _icon)
{
    message_box_->setIcon(_icon);
    message_box_->setText(_message);
    message_box_->open();
}

void main_window::create_actions_menus()
{
#define X(_v) QIcon::fromTheme(QIcon::ThemeIcon::_v)
#define Y(_v) QKeySequence::_v

    file_menu_ = menuBar()->addMenu("File");
    file_menu_->addAction(X(DocumentNew), "New...", Y(New), create_fd_, &create_file_dialog::open);
    file_menu_->addSeparator();
    file_menu_->addAction(X(DocumentOpen), "Open...", Y(Open), open_fd_, qOverload<>(&QFileDialog::open));
    file_menu_->addSeparator();
    save_ = file_menu_->addAction(X(DocumentSave), "Save", Y(Save), canv_, &canvas::save);
    save_as_ = file_menu_->addAction(X(DocumentSaveAs), "Save As...", Y(SaveAs), canv_, qOverload<>(&canvas::save_as));
    file_menu_->addSeparator();
    close_ = file_menu_->addAction(X(WindowClose), "Close", Y(Close), canv_, &canvas::close_image);
    file_menu_->addSeparator();
    file_menu_->addAction(X(ApplicationExit), "Exit", Y(Quit), this, &main_window::close);

    edit_menu_ = menuBar()->addMenu("Edit");
    undo_ = edit_menu_->addAction(X(EditUndo), "Undo", Y(Undo), canv_, &canvas::undo);
    redo_ = edit_menu_->addAction(X(EditRedo), "Redo", Y(Redo), canv_, &canvas::redo);
    edit_menu_->addSeparator();
    edit_menu_->addAction("Swap Pen Colors", QKeySequence{"X"}, canv_, &canvas::swap_colors);

    view_menu_ = menuBar()->addMenu("View");
    zoom_in_ = view_menu_->addAction(X(ZoomIn), "Zoom In", Y(ZoomIn), canv_, &canvas::zoom_in);
    zoom_out_ = view_menu_->addAction(X(ZoomOut), "Zoom Out", Y(ZoomOut), canv_, &canvas::zoom_out);
    view_menu_->addSeparator();
    default_zoom_ = view_menu_->addAction("Default Zoom", canv_, &canvas::default_zoom);

#undef Y
#undef X

    view_ = make_tool_action(tool_group_, "View", {"W"}, ":/assets/icons/view.png", tool::view);
    pen_ = make_tool_action(tool_group_, "Pen", {"D"}, ":/assets/icons/pen.png", tool::pen);
    connect(tool_group_, &QActionGroup::triggered,
        [this](QAction *_act) { canv_->choose_tool(qvariant_cast<tool::type>(_act->data())); });
    view_->trigger();

    connect(palette_tab_, &palette_tab::color_selected, canv_, &canvas::set_color);
    connect(canv_, &canvas::color_updated, palette_tab_, &palette_tab::set_color);
    connect(canv_, &canvas::color_updated, palette_action_,
        [this]() { palette_action_->set_colors(canv_->color(true), canv_->color(false)); });

    connect(palette_action_, &palette_action::toggled, palette_tab_, &palette_tab::setVisible);
    connect(palette_tab_, &palette_tab::visibilityChanged, palette_action_, &palette_action::setChecked);

    tool_menu_ = menuBar()->addMenu("Tool");
    tool_menu_->addActions(tool_group_->actions());

    auto *tool_bar_ = new QToolBar("Tool Bar");
    tool_bar_->setMovable(false);
    tool_bar_->setIconSize(view_->icon().availableSizes()[0]);
    tool_bar_->addActions(tool_group_->actions());
    tool_bar_->addSeparator();
    tool_bar_->addAction(palette_action_);
    addToolBar(tool_bar_);
}
} // namespace mytec