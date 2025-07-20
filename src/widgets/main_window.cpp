#include "main_window.hpp"

#include "palette_tab.hpp"
#include "util/str.hpp"

namespace
{
QIcon icon_from_atlas(const QPixmap& _atlas, const std::initializer_list<std::pair<QIcon::Mode, QIcon::State>>& _modes)
{
    QIcon result;
    const auto dim = _atlas.height();
    int shift = 0;
    for (auto [mode, state] : _modes)
        result.addPixmap(_atlas.copy(dim * (shift++), 0, dim, dim), mode, state);
    return result;
}

QAction* make_tool_action(
    QAction* _target, const QKeySequence& _shortcut, const QString& _atlas, mytec::tool::type _type)
{
    auto icon = icon_from_atlas(_atlas, {{QIcon::Normal, QIcon::Off}, {QIcon::Normal, QIcon::On}});
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
      create_file_dialog_(new create_file_dialog(this)),
      message_box_(new QMessageBox(QMessageBox::Icon::Information, {}, {}, QMessageBox::StandardButton::Ok, this))
{
    setWindowTitle("Mytec");
    setMinimumSize(480, 320);
    setWindowIcon(QIcon{":/assets/logo-128.png"});
    load_style();

    create_actions_menus();
    setCentralWidget(canv_);

    connect(create_file_dialog_, &create_file_dialog::confirmed, canv_, &canvas::create_image);
    connect(canv_, &canvas::changed, this, &main_window::on_active_editor_changed);
    connect(canv_, &canvas::failed, this, [this](const auto& _msg) { show_message(QMessageBox::Warning, _msg); });
    statusBar()->addWidget(status_bar_);

    canv_->bootstrap();
}

void main_window::on_active_editor_changed(editor* const _editor)
{
    const bool enabled = _editor;
    for (auto* a : QList{save_as_, close_, default_zoom_})
        a->setEnabled(enabled);

    save_->setEnabled(enabled && (!_editor->exists() || !_editor->history()->isClean()));
    zoom_out_->setEnabled(enabled && _editor->zoom() != editor::zoom_bounds.first);
    zoom_in_->setEnabled(enabled && _editor->zoom() != editor::zoom_bounds.second);
    undo_->setEnabled(enabled && _editor->history()->canUndo());
    redo_->setEnabled(enabled && _editor->history()->canRedo());

    QString text;
    if (enabled)
    {
        const auto& img = _editor->get_image();
        const auto [w, h] = img.size();
        const auto* const palette = img.get_palette();
        text = QString("%0%1 (%2) | %3x%4 | %5%")
                   .arg(ellipsis(_editor->name(), 50))
                   .arg("* "[_editor->history()->isClean()])
                   .arg(palette ? palette->name() : "32bpp")
                   .arg(w)
                   .arg(h)
                   .arg(static_cast<int>(_editor->zoom() * 100));
    }
    status_bar_->setText(text);
}

void main_window::on_tool_active_changed(bool _engaged) { menuBar()->setDisabled(_engaged); }

void main_window::open()
{
    // TODO
}

void main_window::exit()
{
    // TODO
}

void main_window::show_message(QMessageBox::Icon _icon, const QString& _message)
{
    message_box_->setIcon(_icon);
    message_box_->setText(_message);
    message_box_->open();
}

void main_window::load_style()
{
    QPalette palette;
    palette.setColor(QPalette::Window, qRgb(0x1D, 0x1D, 0x26));     // very dark gray
    palette.setColor(QPalette::WindowText, qRgb(0xFA, 0xF8, 0xE8)); // pale yellow
    // primary qRgb(0x3E, 0x43, 0x48) // dark grayish blue
    // tertiary qRgb(0x46, 0x8C, 0x4D) // dark lime green
    setAutoFillBackground(true);
    setPalette(palette);
}

void main_window::create_actions_menus()
{
#define X(_v) QIcon::fromTheme(QIcon::ThemeIcon::_v)
#define Y(_v) QKeySequence::_v

    file_menu_ = menuBar()->addMenu("File");
    create_ = file_menu_->addAction(X(DocumentNew), "New...", Y(New), create_file_dialog_, &create_file_dialog::open);
    file_menu_->addSeparator();
    open_ = file_menu_->addAction(X(DocumentOpen), "Open...", Y(Open), this, &main_window::open);
    file_menu_->addSeparator();
    save_ = file_menu_->addAction(X(DocumentSave), "Save", Y(Save), canv_, &canvas::save);
    save_as_ = file_menu_->addAction(X(DocumentSaveAs), "Save As...", Y(SaveAs), canv_, qOverload<>(&canvas::save_as));
    file_menu_->addSeparator();
    close_ = file_menu_->addAction(X(WindowClose), "Close", Y(Close), canv_, &canvas::close_image);
    file_menu_->addSeparator();
    exit_ = file_menu_->addAction(X(ApplicationExit), "Exit", Y(Quit), this, &main_window::exit);

    edit_menu_ = menuBar()->addMenu("Edit");
    undo_ = edit_menu_->addAction(X(EditUndo), "Undo", Y(Undo), canv_, &canvas::undo);
    redo_ = edit_menu_->addAction(X(EditRedo), "Redo", Y(Redo), canv_, &canvas::redo);
    edit_menu_->addSeparator();
    swap_colors_ = edit_menu_->addAction("Swap Pen Colors", QKeySequence{"X"}, canv_, &canvas::swap_colors);

    view_menu_ = menuBar()->addMenu("View");
    zoom_in_ = view_menu_->addAction(X(ZoomIn), "Zoom In", Y(ZoomIn), canv_, &canvas::zoom_in);
    zoom_out_ = view_menu_->addAction(X(ZoomOut), "Zoom Out", Y(ZoomOut), canv_, &canvas::zoom_out);
    view_menu_->addSeparator();
    default_zoom_ = view_menu_->addAction("Default Zoom", canv_, &canvas::default_zoom);

#undef Y
#undef X

    tool_group_ = new QActionGroup(this);
    view_ = make_tool_action(tool_group_->addAction("View"), {"W"}, ":/assets/icons/view.png", tool::view);
    pen_ = make_tool_action(tool_group_->addAction("Pen"), {"D"}, ":/assets/icons/pen.png", tool::pen);
    connect(tool_group_, &QActionGroup::triggered,
        [this](auto* _act) { canv_->on_tool_chosen(qvariant_cast<tool::type>(_act->data())); });
    view_->trigger();

    palette_toggle_ = new palette_action(this);
    palette_tab_ = new palette_tab(this);
    connect(canv_, &canvas::colors_changed, palette_toggle_, &palette_action::change_colors);
    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, palette_tab_);
    connect(palette_toggle_, &palette_action::toggled, palette_tab_, &palette_tab::setVisible);
    connect(palette_tab_, &palette_tab::visibilityChanged, palette_toggle_, &palette_action::setChecked);

    tool_menu_ = menuBar()->addMenu("Tool");
    tool_menu_->addActions(tool_group_->actions());

    auto* tool_bar_ = new QToolBar("Tool Bar");
    tool_bar_->setMovable(false);
    tool_bar_->setIconSize(view_->icon().availableSizes()[0]);
    tool_bar_->addActions(tool_group_->actions());
    tool_bar_->addSeparator();
    tool_bar_->addAction(palette_toggle_);
    addToolBar(tool_bar_);
}
} // namespace mytec