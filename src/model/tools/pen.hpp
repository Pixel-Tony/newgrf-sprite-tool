#ifndef MYTEC_PEN_HPP
#define MYTEC_PEN_HPP

#include "tool.hpp"

#include <headers.hpp>

namespace mytec
{
class pen final : public tool
{
public:
    class command final : public QUndoCommand
    {
    public:
        command(bool _left_btn, QColor _color, editor* _editor, image* _image);

        [[nodiscard]] bool is_empty() const noexcept;
        void undo() override;
        void redo() override;
        void append(QPoint _p, QColor _col);

        const QColor color_;

    private:
        const bool left_btn_;
        editor* const editor_;
        image* const image_;
        std::vector<std::pair<QPoint, QColor>> pixels_;
    };

    pen();
    ~pen() override;

    bool event(QEvent& _ev, editor& _editor, image& _image) override;

private:
    command* command_ = nullptr;
};
} // namespace mytec

#endif // MYTEC_PEN_HPP