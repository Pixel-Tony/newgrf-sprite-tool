#ifndef MYTEC_PEN_HPP
#define MYTEC_PEN_HPP

#include "tool.hpp"

namespace mytec
{
class pen final : public tool
{
private:
    struct command final : public QUndoCommand
    {
        command(QColor _color, image* _image);

        void undo() override;
        void redo() override;
        void add_color(QPointF _pos);

        const QColor color_;
        image* const image_;
        std::vector<std::pair<QPoint, QColor>> pixels_;
    };

public:
    pen();
    ~pen() override;

    bool editor_event(QEvent& _ev, editor& _editor) override;

    void image_mouse_move_event(QGraphicsSceneMouseEvent& _ev, canvas& _canv, editor& _ed) override;
    void image_mouse_press_event(QGraphicsSceneMouseEvent& _ev, canvas& _canv, editor& _ed) override;
    void image_mouse_release_event(QGraphicsSceneMouseEvent& _ev, canvas& _canv, editor& _ed) override;

private:
    command* command_ = nullptr;
};
} // namespace mytec

#endif // MYTEC_PEN_HPP