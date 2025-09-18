#include "canvas.hpp"

#include "model/tools/tool_factory.hpp"

namespace
{
const auto err_could_not_save = "Couldn't save the image.";
const auto save_as_fd_key = "canvas/saveFileDialog/state";
} // namespace

namespace mytec
{
canvas::canvas(QWidget *_parent)
    : QTabWidget(_parent),
      save_as_fd_(new QFileDialog(this, "Choose file", {}, "*.png")),
      save_file_dialog_(new save_file_dialog(this))
{
    tool::current = tool_factory::make(tool::view);
    save_as_fd_->setWindowTitle("Choose file to save as");
    save_as_fd_->setNameFilter("Image Files (*.png)");
    save_as_fd_->setAcceptMode(QFileDialog::AcceptSave);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setContentsMargins({});

    connect(this, &canvas::changed,
        [this](editor *_ed)
        {
            auto *t = tool::current;
            t->exit_editor(last_editor_);
            last_editor_ = _ed;
            t->enter_editor(_ed);
            if (!_ed)
                return;
            const auto i = currentIndex();
            setTabText(i, _ed->name() + "* "[_ed->history()->isClean()]);
            setTabToolTip(i, _ed->path());
        });
    connect(this, &QTabWidget::currentChanged, [this] { emit changed(current_editor()); });
}

canvas::~canvas() { delete tool::current; }

void canvas::bootstrap()
{
    emit changed(nullptr);
    emit color_updated(primary_, true);
    emit color_updated(secondary_, false);
}

void canvas::add_editor(editor *const _ed)
{
    connect(_ed, &editor::changed, [this, _ed] { emit changed(_ed); });
    const auto &img = _ed->get_image();
    connect(&img, &image::mouse_moved, this,
        [this, _ed](auto &_ev) { tool::current->image_mouse_move_event(_ev, *this, *_ed); });
    connect(&img, &image::mouse_pressed, this,
        [this, _ed](auto &_ev) { tool::current->image_mouse_press_event(_ev, *this, *_ed); });
    connect(&img, &image::mouse_released, this,
        [this, _ed](auto &_ev) { tool::current->image_mouse_release_event(_ev, *this, *_ed); });
    setCurrentIndex(addTab(_ed, _ed->name()));
}

void canvas::create_image(const QString &_name, QSize _size, palette::type _palette)
{
    add_editor(new editor(QString{_name}, _size, _palette));
}

void canvas::open_image(const QString &_filepath)
{
    for (int i = 0, c = count(); i < c; ++i)
    {
        if (qobject_cast<editor *>(widget(i))->path() == _filepath)
        {
            setCurrentIndex(i);
            return;
        }
    }
    try
    {
        add_editor(new editor(_filepath));
    }
    catch (std::runtime_error &err)
    {
        emit failed(err.what());
    }
}

void canvas::save()
{
    auto *const ed = current_editor();
    if (!ed || (ed->history()->isClean() && ed->exists()))
        return;

    if (!ed->exists())
    {
        save_as(ed);
        return;
    }
    if (!ed->save())
    {
        emit failed(err_could_not_save);
        return;
    }
}

void canvas::save_as(editor *_ed)
{
    disconnect(fp_conn_);
    fp_conn_ = connect(
        save_as_fd_, &QFileDialog::accepted, [this, _ed] { fd_on_accepted(save_as_fd_->selectedFiles().data(), _ed); });

    if (_ed->exists())
        save_as_fd_->setDirectory(_ed->path());
    save_as_fd_->open();
}

void canvas::save_as()
{
    if (auto *ed = current_editor())
        save_as(ed);
}

void canvas::close_editor(editor *_ed, int _index)
{
    removeTab(_index != -1 ? _index : currentIndex());
    _ed->close();
}

void canvas::close_image()
{
    auto *ed = current_editor();
    if (!ed)
        return;
    if (ed->history()->isClean())
    {
        close_editor(ed);
        return;
    }
    disconnect(sfd_conn_);
    sfd_conn_ = connect(save_file_dialog_, &save_file_dialog::accepted,
        [this, ed](const QString *_path) { fd_on_accepted(_path, ed, true); });

    save_file_dialog_->open(ed->name(), ed->path());
}

bool canvas::try_exit()
{
    while (count() > 0)
    {
        auto *ed = qobject_cast<editor *>(widget(count() - 1));
        if (ed->history()->isClean())
        {
            close_editor(ed);
            continue;
        }

        disconnect(sfd_conn_);
        sfd_conn_ = connect(save_file_dialog_, &save_file_dialog::accepted,
            [this, ed](const QString *_path) { fd_on_accepted(_path, ed, true, true); });

        if (save_file_dialog_->isHidden())
            save_file_dialog_->open(ed->name(), ed->path());
        return false;
    }
    return true;
}

void canvas::undo()
{
    auto *ed = current_editor();
    if (!ed)
        return;
    if (auto *h = ed->history(); h->canUndo())
        h->undo();
}

void canvas::redo()
{
    auto *ed = current_editor();
    if (!ed)
        return;
    if (auto *h = ed->history(); h->canRedo())
        h->redo();
}

void canvas::swap_colors()
{
    std::swap(primary_, secondary_);
    emit color_updated(primary_, true);
    emit color_updated(secondary_, false);
}

void canvas::zoom_in()
{
    if (auto *ed = current_editor())
        ed->zoom_in();
}

void canvas::zoom_out()
{
    if (auto *ed = current_editor())
        ed->zoom_out();
}

void canvas::default_zoom()
{
    if (auto *ed = current_editor())
        ed->default_zoom();
}

void canvas::choose_tool(const tool::type _type)
{
    auto *t = tool::current;
    if (t->type_ == _type)
        return;
    auto *next = tool_factory::make(_type);
    auto *ed = current_editor();
    t->exit_editor(ed);
    delete t;
    t = tool::current = next;
    t->enter_editor(ed);
    emit changed(ed);
}

editor *canvas::current_editor() const noexcept { return qobject_cast<editor *>(currentWidget()); }

bool canvas::is_path_occupied(const QString &_path, const editor *_editor) const noexcept
{
    for (int i = 0, mx = count(); i < mx; ++i)
    {
        const auto *const ed = qobject_cast<editor *>(widget(i));
        if (ed->path() == _path && ed != _editor)
            return true;
    }
    return false;
}

void canvas::fd_on_accepted(const QString *_path, editor *_ed, bool _close, bool _try_exit)
{
    if (_path)
    {
        if (is_path_occupied(*_path, _ed))
        {
            emit failed("File is already open in the editor.");
            return;
        }
        if (!_ed->save(*_path))
        {
            emit failed(err_could_not_save);
            return;
        }
    }
    if (!_close)
        return;
    save_file_dialog_->close();
    close_editor(_ed);
    if (_try_exit && try_exit())
        emit exit_prepared();
}

QColor canvas::color(bool _primary) const noexcept { return _primary ? primary_ : secondary_; }

void canvas::set_color(QColor _color, bool _primary)
{
    (_primary ? primary_ : secondary_) = _color;
    emit color_updated(_color, _primary);
}

void canvas::save_gui_state(QSettings &_settings) { _settings.setValue(save_as_fd_key, save_as_fd_->saveState()); }

void canvas::load_gui_state(QSettings &_settings)
{
    if (auto v = _settings.value(save_as_fd_key); !v.isNull())
        save_as_fd_->restoreState(v.toByteArray());
}

} // namespace mytec