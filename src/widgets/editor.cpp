#include "editor.hpp"

#include "model/image.hpp"
#include "model/tools/tool.hpp"

namespace
{
QString name_from_path(const QString &_path)
{
    auto value = QFileInfo{_path}.fileName();
    const auto slice_len = value.length() - 4;
    return value.endsWith(".png", Qt::CaseInsensitive) && slice_len > 0 ? value.slice(0, slice_len) : value;
}
} // namespace

namespace mytec
{
editor::editor(QString &&_name, QSize _image_size, palette::type _palette, QWidget *_parent)
    : editor(new image(_image_size, _palette), {}, std::move(_name), _parent)
{
}

editor::editor(const QString &_filepath, QWidget *_parent)
    : editor(new image(_filepath), QString{_filepath}, std::move(name_from_path(_filepath)), _parent)
{
}

editor::editor(image *_image, QString &&_path, QString &&_name, QWidget *_parent)
    : QGraphicsView(_parent),
      image_(_image),
      name_(std::move(_name)),
      path_(std::move(_path)),
      history_(new QUndoStack(this))
{
    setViewport(new QOpenGLWidget(this));
    setBackgroundBrush(palette().window());

    setFocusPolicy(Qt::WheelFocus);
    setAttribute(Qt::WA_DeleteOnClose);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setContentsMargins(2, 2, 2, 2);

    connect(this, &editor::changed, &editor::update_position_scale);
    connect(history_, &QUndoStack::indexChanged, this, &editor::changed);
    connect(history_, &QUndoStack::cleanChanged, this, &editor::changed);

    setTransformationAnchor(ViewportAnchor::AnchorUnderMouse);
    scene_.addItem(image_);
    setScene(&scene_);
}

editor::~editor() { history_->disconnect(this); }

image &editor::get_image() noexcept { return *image_; }

const image &editor::get_image() const noexcept { return *image_; }

const QString &editor::name() const noexcept { return name_; }

const QString &editor::path() const noexcept { return path_; }

QUndoStack *editor::history() noexcept { return history_; }

const QUndoStack *editor::history() const noexcept { return history_; }

bool editor::exists() const noexcept { return !path_.isEmpty(); }

float editor::zoom() const noexcept { return zoom_; }

bool editor::event(QEvent *_ev) { return tool::current->editor_event(*_ev, *this) || QGraphicsView::event(_ev); }

void editor::update_position_scale()
{
    const auto p = mapToScene(position_);
    setTransform(QTransform().translate(p.x(), p.y()).scale(zoom_, zoom_));
}

void editor::wheelEvent(QWheelEvent *_ev)
{
    _ev->accept();
    const auto sign = 2 * (_ev->angleDelta().y() > 0) - 1;
    const auto p = _ev->position().toPoint();
    apply_zoom_delta(sign * zoom_factor(), p);
}

int editor::zoom_factor() const noexcept
{
    return (1 + (zoom_ >= zoom_bounds.second / 3)) * (qApp->keyboardModifiers().testFlag(Qt::ControlModifier) + 1);
}

void editor::zoom_in() { apply_zoom_delta(zoom_factor(), rect().center()); }

void editor::zoom_out() { apply_zoom_delta(-zoom_factor(), rect().center()); }

void editor::default_zoom()
{
    zoom_ = 1.f;
    emit changed();
}

bool editor::save() { return save(path_); }

bool editor::save(const QString &_path)
{
    if (!image_->save_with_palette(_path, "png"))
        return false;
    if (path_ != _path)
    {
        path_ = _path;
        name_ = name_from_path(path_);
    }
    if (!history_->isClean())
        history_->setClean();
    else
        emit changed();
    return true;
}

void editor::apply_zoom_delta(const float _delta, const QPoint _center)
{
    const auto new_zoom = std::clamp(zoom_ + _delta, zoom_bounds.first, zoom_bounds.second);
    if (new_zoom == zoom_)
        return;
    position_ = (position_ - _center) * (new_zoom / zoom_) + _center; // a homothety with given center
    zoom_ = new_zoom;
    emit changed();
}

void editor::mousePressEvent(QMouseEvent *_ev)
{
    if (!tool::current->editor_event(*_ev, *this))
        QGraphicsView::mousePressEvent(_ev);
}

void editor::mouseReleaseEvent(QMouseEvent *_ev)
{
    if (!tool::current->editor_event(*_ev, *this))
        QGraphicsView::mouseReleaseEvent(_ev);
}

void editor::mouseMoveEvent(QMouseEvent *_ev)
{
    if (!tool::current->editor_event(*_ev, *this))
        QGraphicsView::mouseMoveEvent(_ev);
}
} // namespace mytec
