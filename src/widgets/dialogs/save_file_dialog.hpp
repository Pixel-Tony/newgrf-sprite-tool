#ifndef MYTEC_SAVE_FILE_DIALOG_HPP
#define MYTEC_SAVE_FILE_DIALOG_HPP

#include <headers.hpp>

namespace mytec
{
class save_file_dialog : public QDialog
{
    Q_OBJECT

public:
    save_file_dialog(QWidget* _parent = nullptr);

    void open(const QString& _name, const QString& _path);

signals:
    void accepted(const QString* _path);

private:
    QLabel* const info_;
    QFileDialog* const file_picker_;
};
} // namespace mytec

#endif // MYTEC_SAVE_FILE_DIALOG_HPP