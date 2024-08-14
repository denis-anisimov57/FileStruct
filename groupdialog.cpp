#include "groupdialog.h"
#include "ui_groupdialog.h"

GroupDialog::GroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupDialog)
{
    ui->setupUi(this);
    QList<QAbstractButton*> buttons = ui->buttonBox->buttons();
    buttons[0]->setText("Ок");
    buttons[1]->setText("Отмена");

    connect(ui->TagAddButton, &QPushButton::clicked, this, &GroupDialog::addTagToList);
    connect(ui->TagRemoveButton, &QPushButton::clicked, this, &GroupDialog::removeTagFromList);
    connect(this, &QDialog::accepted, this, &GroupDialog::sendTagList);
}

void GroupDialog::setNewTags(QStringList tags) {
    ui->TagsBox->clear();
    ui->TagsBox->addItems(tags);
}

void GroupDialog::addTagToList() {
    if(ui->TagsList->findItems(ui->TagsBox->currentText(), Qt::MatchExactly).empty()) {
        ui->TagsList->addItem(ui->TagsBox->currentText());
    }
}

void GroupDialog::removeTagFromList() {
    ui->TagsList->takeItem(ui->TagsList->currentRow());
    ui->TagsList->update();
}

void GroupDialog::sendTagList() {
    int c = ui->TagsList->count();
    //QString?
    std::vector<std::string> tags;
    for(int i = 0; i < c; i++) {
        tags.push_back(ui->TagsList->item(i)->text().toStdString());
    }
    emit dialogRes(tags);
}

GroupDialog::~GroupDialog()
{
    delete ui;
}
