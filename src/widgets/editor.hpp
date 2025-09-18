#ifndef MYTEC_CANVAS_SCENE_HPP
#define MYTEC_CANVAS_SCENE_HPP

#include "model/image.hpp"
#include "model/palette.hpp"
#include "model/tools/tool.hpp"

namespace mytec
{
class editor : public QGraphicsView
{
    Q_OBJECT

public:
    editor(QString &&_name, QSize _image_size, palette::type _palette, QWidget *_parent = nullptr);

    editor(const QString &_filepath, QWidget *_parent = nullptr);

    static constexpr QPair<float, float> zoom_bounds = {1, 32};

    ~editor() override;

    [[nodiscard]] image &get_image() noexcept;
    [[nodiscard]] const image &get_image() const noexcept;
    [[nodiscard]] const QString &name() const noexcept;
    [[nodiscard]] const QString &path() const noexcept;
    [[nodiscard]] QUndoStack *history() noexcept;
    [[nodiscard]] const QUndoStack *history() const noexcept;
    [[nodiscard]] bool exists() const noexcept;
    [[nodiscard]] float zoom() const noexcept;

    void zoom_in();
    void zoom_out();
    void default_zoom();

    bool save();
    bool save(const QString &_path);

signals:
    void changed();

protected:
    bool event(QEvent *_ev) override;
    void wheelEvent(QWheelEvent *_ev) override;

    void mousePressEvent(QMouseEvent *_ev) override;

    void mouseReleaseEvent(QMouseEvent *_ev) override;

    void mouseMoveEvent(QMouseEvent *_ev) override;

private:
    editor(image *_image, QString &&_path, QString &&_name, QWidget *_parent);

    void apply_zoom_delta(float _delta, QPoint _center);
    void update_position_scale();
    [[nodiscard]] int zoom_factor() const noexcept;

    QGraphicsScene scene_;
    image *image_;
    QString name_;
    QString path_;
    float zoom_ = 1;
    QPoint position_;

    QUndoStack *history_;
};
} // namespace mytec

#endif // MYTEC_CANVAS_SCENE_HPP