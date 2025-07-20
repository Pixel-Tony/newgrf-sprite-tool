#ifndef MYTEC_CANVAS_HPP
#define MYTEC_CANVAS_HPP

#include "dialogs/save_file_dialog.hpp"
#include "editor.hpp"
#include "model/palette.hpp"
#include "model/tools/tool.hpp"

#include <headers.hpp>

namespace mytec
{
class canvas : public QTabWidget
{
    Q_OBJECT

public:
    explicit canvas(QWidget* _parent = nullptr);

    ~canvas();

    void bootstrap();

signals:
    void changed(editor* _editor) const;
    void failed(const QString& _message) const;
    void colors_changed(QColor _primary, QColor _secondary);

public slots:
    void create_image(const QString& _name, QSize _size, palette::type _palette);
    void save();
    void save_as();
    void close_image();

    void undo();
    void redo();
    void swap_colors();

    void zoom_in();
    void zoom_out();
    void default_zoom();

    void on_tool_chosen(tool::type _type);

private:
    [[nodiscard]] editor* current_editor() const noexcept;

    void close_editor(editor* _ed, int _index = -1);

    void save_as(editor* _ed);
    [[nodiscard]] bool is_path_occupied(const QString& _path, const editor* _editor) const noexcept;

    QColor primary_ = qRgb(255, 255, 255);
    QColor secondary_ = qRgb(0, 0, 255);
    tool* tool_ = nullptr;

    QFileDialog* file_picker_;
    QMetaObject::Connection fp_conn_;
    save_file_dialog* save_file_dialog_;
    QMetaObject::Connection sfd_conn1_;
    QMetaObject::Connection sfd_conn2_;
};

} // namespace mytec

#endif // MYTEC_CANVAS_HPP