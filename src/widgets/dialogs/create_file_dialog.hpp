#ifndef MYTEC_CREATE_FILE_DIALOG_HPP
#define MYTEC_CREATE_FILE_DIALOG_HPP

#include "model/palette.hpp"

#include <headers.hpp>

namespace mytec
{
class create_file_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit create_file_dialog(QWidget* _parent = nullptr);

public slots:
    void open() override;

signals:
    void confirmed(const QString& _name, QSize _size, palette::type _palette) const;

private slots:
    void create() const;
    void validate();

private:
    QLabel* file_name_lbl_;
    QLineEdit* file_name_;

    QLabel* width_lbl_;
    QLineEdit* width_;

    QLabel* height_lbl_;
    QLineEdit* height_;

    QLabel* palette_lbl_;
    QComboBox* palette_;

    QPushButton* create_;
    QPushButton* cancel_;

    QIntValidator int_validator_;
    QRegularExpressionValidator file_name_validator_;
};
} // namespace mytec

#endif // MYTEC_CREATE_FILE_DIALOG_HPP