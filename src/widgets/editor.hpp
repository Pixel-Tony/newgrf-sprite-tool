#ifndef MYTEC_CANVAS_SCENE_HPP
#define MYTEC_CANVAS_SCENE_HPP
#include "model/image.hpp"
#include "model/palette.hpp"
#include "model/tools/tool.hpp"

namespace mytec
{
class editor : public QWidget
{
    Q_OBJECT

public:
    editor(QString _name, QSize _image_size, palette::type _palette, tool* const volatile* _tool,
        QWidget* _parent = nullptr);

    static constexpr QPair<float, float> zoom_bounds = {1, 32};

    [[nodiscard]] const image& get_image() const noexcept;
    [[nodiscard]] const QString& name() const noexcept;
    [[nodiscard]] const QString& path() const noexcept;
    [[nodiscard]] QUndoStack& history() noexcept;
    [[nodiscard]] bool exists() const noexcept;
    [[nodiscard]] float zoom() const noexcept;

    [[nodiscard]] QPoint image_pos() const noexcept;
    void set_image_pos(QPoint _pos);

    void zoom_in();
    void zoom_out();
    void default_zoom();

    bool save();
    bool save(const QString& _path);

signals:
    void changed(editor* _editor);

protected:
    bool event(QEvent* _ev) override;
    void paintEvent(QPaintEvent*) override;
    void wheelEvent(QWheelEvent* _event) override;

private:
    [[nodiscard]] bool apply_zoom_delta(float _delta, QPoint _center);
    QUndoStack history_;
    image image_;
    QString name_;
    QString path_;
    float zoom_ = 1;
    QPoint position_;
    bool should_center_ = true;
    tool* const volatile* const tool_;
};
} // namespace mytec

#endif // MYTEC_CANVAS_SCENE_HPP