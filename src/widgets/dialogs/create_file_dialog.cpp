#include "create_file_dialog.hpp"

namespace
{
constexpr auto dim_min = 1;
constexpr auto dim_max = 5000;
const auto file_name_regex = QRegularExpression{R"([^<>:'"/\\|?*]+)"};
const auto palette_options = QStringList{"DOS", "Windows (Legacy)"};
} // namespace

namespace mytec
{
create_file_dialog::create_file_dialog(QWidget* _parent)
    : QDialog(_parent),
      int_validator_(dim_min, dim_max),
      file_name_validator_(file_name_regex),
      file_name_lbl_(new QLabel("File Name")),
      file_name_(new QLineEdit("Unnamed")),
      width_lbl_(new QLabel("Width")),
      width_(new QLineEdit),
      height_lbl_(new QLabel("Height")),
      height_(new QLineEdit),
      palette_lbl_(new QLabel("Palette")),
      palette_(new QComboBox),
      create_(new QPushButton("Create")),
      cancel_(new QPushButton("Cancel"))
{
    setWindowTitle("Create File");
    setModal(true);

    for (auto* input : QList<QWidget*>{file_name_, width_, height_, palette_})
        input->setFixedWidth(240);

    file_name_->setValidator(&file_name_validator_);
    width_->setValidator(&int_validator_);
    height_->setValidator(&int_validator_);
    palette_->addItems(palette_options);

    auto* buttons = new QHBoxLayout;
    buttons->addWidget(create_);
    buttons->addWidget(cancel_);

    auto* table = new QFormLayout;
    table->addRow(file_name_lbl_, file_name_);
    table->addRow(width_lbl_, width_);
    table->addRow(height_lbl_, height_);
    table->addRow(palette_lbl_, palette_);

    table->addRow(buttons);
    setLayout(table);

    connect(file_name_, &QLineEdit::textChanged, this, &create_file_dialog::validate);
    connect(width_, &QLineEdit::textChanged, this, &create_file_dialog::validate);
    connect(height_, &QLineEdit::textChanged, this, &create_file_dialog::validate);

    connect(create_, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancel_, &QPushButton::clicked, this, &QDialog::reject);

    connect(this, &QDialog::accepted, this, &create_file_dialog::create);

    validate();
}

void create_file_dialog::validate()
{
    QString tooltip;
    if (!file_name_->hasAcceptableInput())
        tooltip = "Invalid file name";
    else if (!width_->hasAcceptableInput())
        tooltip = "Invalid width";
    else if (!height_->hasAcceptableInput())
        tooltip = "Invalid height";

    create_->setEnabled(tooltip.isEmpty());
    create_->setToolTip(tooltip);
}

void create_file_dialog::open()
{
    file_name_->setText("Unnamed");
    width_->setText("100");
    height_->setText("100");
    palette_->setCurrentIndex(0);
    this->QDialog::open();
}

void create_file_dialog::create() const
{
    const auto name = file_name_->text();
    const auto size = QSize{width_->text().toInt(), height_->text().toInt()};
    const auto palette = static_cast<palette::type>(palette_->currentIndex());
    emit confirmed(name, size, palette);
}
} // namespace mytec
