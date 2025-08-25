#include "canvas.hpp"

#include "model/tools/tool_factory.hpp"

namespace
{
const auto err_could_not_save = "Couldn't save the image.";
const auto err_file_already_open = "File is already open in the editor.";
} // namespace

namespace mytec
{
canvas::canvas(QWidget* _parent)
    : QTabWidget(_parent),
      file_picker_(new QFileDialog(this, "Choose file", {}, "*.png")),
      save_file_dialog_(new save_file_dialog(this))
{
    file_picker_->setWindowTitle("Choose file to open");
    file_picker_->setNameFilter("Image Files (*.png)");
    file_picker_->setAcceptMode(QFileDialog::AcceptSave);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setContentsMargins({});

    connect(this, &canvas::changed,
        [this](editor* _ed)
        {
            if (!_ed)
                return;
            const auto i = currentIndex();
            setTabText(i, _ed->name() + "* "[_ed->history()->isClean()]);
            setTabToolTip(i, _ed->path());
        });
    connect(this, &QTabWidget::currentChanged, [this] { emit changed(current_editor()); });
}

canvas::~canvas() { delete tool_; }

void canvas::bootstrap()
{
    emit changed(nullptr);
    emit colors_changed(primary_, secondary_);
}

void canvas::create_image(const QString& _name, QSize _size, palette::type _palette)
{
    auto* const ed = new editor(QString{_name}, _size, _palette, &tool_, &primary_, &secondary_);
    connect(ed, &editor::changed, [this, ed] { emit changed(ed); });
    setUpdatesEnabled(false);
    setCurrentIndex(addTab(ed, _name));
    setUpdatesEnabled(true);
}

void canvas::save()
{
    auto* const ed = current_editor();
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

void canvas::save_as(editor* _ed)
{
    disconnect(fp_conn_);
    fp_conn_ = connect(file_picker_, &QFileDialog::accepted,
        [this, _ed] { fd_on_accepted(file_picker_->selectedFiles().data(), _ed); });

    if (_ed->exists())
        file_picker_->setDirectory(_ed->path());
    file_picker_->open();
}

void canvas::save_as()
{
    if (auto* ed = current_editor())
        save_as(ed);
}

void canvas::close_editor(editor* _ed, int _index)
{
    removeTab(_index != -1 ? _index : currentIndex());
    _ed->close();
}

void canvas::close_image()
{
    auto* ed = current_editor();
    if (!ed)
        return;
    if (ed->history()->isClean())
    {
        close_editor(ed);
        return;
    }
    disconnect(sfd_conn_);
    sfd_conn_ = connect(save_file_dialog_, &save_file_dialog::accepted,
        [this, ed](const QString* _path) { fd_on_accepted(_path, ed, true); });

    save_file_dialog_->open(ed->name(), ed->path());
}

bool canvas::try_exit()
{
    while (count() > 0)
    {
        auto* ed = qobject_cast<editor*>(widget(count() - 1));
        if (ed->history()->isClean())
        {
            close_editor(ed);
            continue;
        }

        disconnect(sfd_conn_);
        sfd_conn_ = connect(save_file_dialog_, &save_file_dialog::accepted,
            [this, ed](const QString* _path) { fd_on_accepted(_path, ed, true, true); });

        if (save_file_dialog_->isHidden())
            save_file_dialog_->open(ed->name(), ed->path());
        return false;
    }
    emit exit_prepared();
    return true;
}

void canvas::undo()
{
    auto* ed = current_editor();
    if (!ed)
        return;
    if (auto* h = ed->history(); h->canUndo())
        h->undo();
}

void canvas::redo()
{
    auto* ed = current_editor();
    if (!ed)
        return;
    if (auto* h = ed->history(); h->canRedo())
        h->redo();
}

void canvas::swap_colors()
{
    const auto tmp = primary_;
    primary_ = secondary_;
    secondary_ = tmp;
    emit colors_changed(primary_, secondary_);
}

void canvas::zoom_in()
{
    if (auto* ed = current_editor())
        ed->zoom_in();
}

void canvas::zoom_out()
{
    if (auto* ed = current_editor())
        ed->zoom_out();
}

void canvas::default_zoom()
{
    if (auto* ed = current_editor())
        ed->default_zoom();
}

void canvas::on_tool_chosen(const tool::type _type)
{
    if (tool_ && tool_->type_ == _type)
        return;
    auto* next = tool_factory::make(_type);
    delete tool_;
    tool_ = next;
    emit changed(current_editor());
}

editor* canvas::current_editor() const noexcept { return qobject_cast<editor*>(currentWidget()); }

bool canvas::is_path_occupied(const QString& _path, const editor* _editor) const noexcept
{
    for (int i = 0, mx = count(); i < mx; ++i)
    {
        const auto* const ed = qobject_cast<editor*>(widget(i));
        if (ed->path() == _path && ed != _editor)
            return true;
    }
    return false;
}

void canvas::fd_on_accepted(const QString* _path, editor* _ed, bool _close, bool _try_exit)
{
    if (_path)
    {
        if (is_path_occupied(*_path, _ed))
        {
            emit failed(err_file_already_open);
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
    if (_try_exit)
        try_exit();
}
} // namespace mytec