#include "save_file_dialog.hpp"

namespace mytec
{
save_file_dialog::save_file_dialog(QWidget* _parent)
    : QDialog(_parent),
      info_(new QLabel),
      file_picker_(new QFileDialog(this, "Choose file", {}, "*.png"))
{
    setWindowTitle("Unsaved file");
    setModal(true);

    file_picker_->setWindowTitle("Choose file");
    file_picker_->setNameFilter("Image files (*.png)");
    file_picker_->setAcceptMode(QFileDialog::AcceptSave);
    connect(file_picker_, &QFileDialog::accepted, [this] { emit accepted(file_picker_->selectedFiles().data()); });

    auto* save = new QPushButton("Save");
    auto* close = new QPushButton("Close");
    auto* cancel = new QPushButton("Cancel");

    connect(save, &QPushButton::clicked, file_picker_, qOverload<>(&QFileDialog::open));
    connect(close, &QPushButton::clicked, [this] { emit accepted(nullptr); });
    connect(cancel, &QPushButton::clicked, this, &save_file_dialog::reject);

    auto* btn_box = new QHBoxLayout;
    btn_box->addWidget(save);
    btn_box->addWidget(close);
    btn_box->addWidget(cancel);

    auto* lyt = new QVBoxLayout;
    lyt->addWidget(info_);
    lyt->addLayout(btn_box);
    setLayout(lyt);
}

void save_file_dialog::open(const QString& _name, const QString& _path)
{
    info_->setText(QString(R"(Image "%0" is unsaved. Save?)").arg(_name));
    file_picker_->setDirectory(_path.isEmpty() ? QDir::home() : QFileInfo{_path}.path());
    QDialog::open();
}
} // namespace mytec