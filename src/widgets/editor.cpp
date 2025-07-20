#include "editor.hpp"

namespace mytec
{
editor::editor(QString _name, QSize _image_size, palette::type _palette, tool* const* _tool, QColor const* _primary,
    QColor const* _secondary, QWidget* _parent)
    : QWidget(_parent),
      name_(std::move(_name)),
      image_(_image_size, _palette),
      tool_(_tool),
      primary_(_primary),
      secondary_(_secondary),
      history_(new QUndoStack(this))
{
    setFocusPolicy(Qt::StrongFocus);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setContentsMargins(2, 2, 2, 2);

    connect(this, &editor::changed, this, qOverload<>(&editor::update));

    connect(history_, &QUndoStack::indexChanged, [this] { emit changed(this); });
    connect(history_, &QUndoStack::cleanChanged, [this] { emit changed(this); });
}

const image& editor::get_image() const noexcept { return image_; }

const QString& editor::name() const noexcept { return name_; }

const QString& editor::path() const noexcept { return path_; }

QUndoStack* editor::history() noexcept { return history_; }

bool editor::exists() const noexcept { return !path_.isEmpty(); }

float editor::zoom() const noexcept { return zoom_; }

QColor editor::primary() const noexcept { return *primary_; }

QColor editor::secondary() const noexcept { return *secondary_; }

bool editor::event(QEvent* _ev) { return ((*tool_)->event(*_ev, *this, image_)) || QWidget::event(_ev); }

void editor::paintEvent(QPaintEvent*)
{
    const auto region = size();
    const auto render_size = image_.size() * zoom_;

    // Separately center image axes if they fit inside canvas region (excluding some padding)
    const auto half_bounds = (region - render_size) / 2.f;
    const auto fits = QPoint{half_bounds.width() > 150.f, half_bounds.height() > 150.f};
    position_.setX(position_.x() * (1 - fits.x()) + fits.x() * half_bounds.width());
    position_.setY(position_.y() * (1 - fits.y()) + fits.y() * half_bounds.height());

    if (should_center_)
    {
        should_center_ = false;
        position_ = QPoint{half_bounds.width(), half_bounds.height()};
    }

    // clamp to preserve at least 100x100 of an image on the screen
    const auto edge_threshold_px = QSize{100, 100};
    const auto a = edge_threshold_px - render_size;
    const auto b = region - edge_threshold_px;
    position_ = {std::clamp(position_.x(), a.width(), b.width()), std::clamp(position_.y(), a.height(), b.height())};

    QPainter{this}.drawImage(QRect(position_, render_size), image_);
}

void editor::wheelEvent(QWheelEvent* _ev)
{
    _ev->accept();
    const auto d = 2 * (_ev->angleDelta().y() > 0) - 1;
    const auto p = _ev->position().toPoint();
    if (apply_zoom_delta(d, p))
        emit changed(this);
}

QPoint editor::image_pos() const noexcept { return position_; }

void editor::set_image_pos(QPoint _pos)
{
    position_ = _pos;
    emit changed(this);
}

void editor::zoom_in()
{
    if (apply_zoom_delta(1, rect().center()))
        emit changed(this);
}

void editor::zoom_out()
{
    if (apply_zoom_delta(-1, rect().center()))
        emit changed(this);
}

void editor::default_zoom()
{
    zoom_ = 1.f;
    should_center_ = true;
    emit changed(this);
}

bool editor::save() { return save(path_); }

bool editor::save(const QString& _path)
{
    static const auto pat = QRegularExpression("\\\\.png$");

    if (!image_.save(_path, "png"))
        return false;
    if (path_ != _path)
    {
        path_ = _path;
        name_ = QFileInfo{_path}.fileName().remove(pat);
    }
    history_->setClean();
    emit changed(this);
    return true;
}

std::optional<QPoint> editor::to_image_coords(QPointF _click) const
{
    const auto fpoint = (_click - (pos() + position_)) / zoom_;
    const auto pixel = QPoint{qFloor(fpoint.x()), qFloor(fpoint.y())}; // toPoint uses qRound, need qFloor
    return image_.rect().contains(pixel) ? std::optional{pixel} : std::nullopt;
}

std::optional<QColor> editor::put_pixel(QPoint _coordinate, QColor _color)
{
    auto old = image_.pixelColor(_coordinate);
    if (old == _color)
        return std::nullopt;
    image_.setPixelColor(_coordinate, _color);
    emit changed(this);
    return old;
}

bool editor::apply_zoom_delta(const float _delta, const QPoint _center)
{
    const auto zoom_new = std::clamp(zoom_ + _delta, zoom_bounds.first, zoom_bounds.second);
    if (zoom_new == zoom_)
        return false;
    position_ = (position_ * zoom_new - _center * _delta) / zoom_; // a homothety with given center
    zoom_ = zoom_new;
    return true;
}
} // namespace mytec
