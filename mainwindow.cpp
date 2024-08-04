#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QInputDialog>
#include <QScrollBar>
#include <QMessageBox>
#include <QVector>
#include <QDialogButtonBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , fileStruct()
    , groupUi(new GroupDialog)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->OpenFolderButton, &QPushButton::clicked, this, &MainWindow::openFolder);
    connect(ui->AddButton, &QPushButton::clicked, this, &MainWindow::addTag);
    connect(ui->FileList, &QListWidget::currentRowChanged, this, &MainWindow::updateDisplay);
    connect(ui->OpenIniButton, &QPushButton::clicked, this, &MainWindow::openIni);
    connect(ui->SaveIniButton, &QPushButton::clicked, this, &MainWindow::saveIni);
    connect(ui->GroupButton, &QPushButton::clicked, this, &MainWindow::showGroupDialog);
    connect(ui->AddExistingButton, &QPushButton::clicked, this, &MainWindow::addExistingTag);
    connect(ui->RemoveButton, &QPushButton::clicked, this, &MainWindow::removeTag);
    connect(groupUi, &GroupDialog::dialogRes, this, &MainWindow::groupFiles);
}

void MainWindow::openFolder() {
    QString folderName =  QFileDialog::getExistingDirectory(this, tr("Open Directory"), ".", QFileDialog::ShowDirsOnly);
    if(folderName.isEmpty()) {
        return;
    }
    ui->TagList->clear();
    ui->FileList->clear();
    fileStruct.openNewFolder(folderName.toStdString());
    qDebug() << "Opening folder: " << folderName;
    std::vector<std::string> files = fileStruct.getFiles();
    for(auto& file : files) {
        if(file == "Tags") {
            continue;
        }
        ui->FileList->addItem(QString::fromStdString(file));
    }
}

void MainWindow::openIni() {
    QString fileName =  QFileDialog::getOpenFileName(this, tr("Open Inifile"), ".", "Inifiles (*.ini)");
    if(fileName.isEmpty()) {
        return;
    }
    ui->TagList->clear();
    ui->FileList->clear();
    fileStruct.openData(fileName.toStdString());
    qDebug() << "Opening ini: " << fileName;
    std::vector<std::string> files = fileStruct.getFiles();
    for(auto& file : files) {
        if(file == "Tags") {
            continue;
        }
        ui->FileList->addItem(QString::fromStdString(file));
    }
}

void MainWindow::addTag() {
    bool ok;
    if(ui->FileList->selectedItems().empty()) {
        QMessageBox msgBox;
        msgBox.setText("No file selected");
        msgBox.exec();
        return;
    }
    QString tag = QInputDialog::getText(this, tr("Add tag"), tr("Enter tag"), QLineEdit::Normal, "", &ok);
    if(ok && !tag.isEmpty()) {
        fileStruct.addTag(ui->FileList->currentItem()->text().toStdString(), tag.toStdString());
        updateDisplay();
    }
}

void MainWindow::addExistingTag() {
    if(ui->FileList->selectedItems().empty()) {
        QMessageBox msgBox;
        msgBox.setText("No file selected");
        msgBox.exec();
        return;
    }
    QStringList QTags;
    std::vector<std::string> tags = fileStruct.getAllTags();
    for(auto& tag : tags) {
        QTags.push_back(QString::fromStdString(tag));
    }
    bool ok = false;
    QString tag = QInputDialog::getItem(this, "Select tag to combine", "Tag: ", QTags, 0, false, &ok);
    if(ok && !tag.isEmpty()) {
        fileStruct.addTag(ui->FileList->currentItem()->text().toStdString(), tag.toStdString());
        updateDisplay();
    }
}

void MainWindow::removeTag() {
    if(ui->FileList->selectedItems().empty()) {
        QMessageBox msgBox;
        msgBox.setText("No file selected");
        msgBox.exec();
        return;
    }
    if(!ui->TagList->count()) {
        QMessageBox msgBox;
        msgBox.setText("No tags");
        msgBox.exec();
        return;
    }
    QStringList QTags;
    std::vector<std::string> tags = fileStruct.getTags(ui->FileList->currentItem()->text().toStdString());
    for(auto& tag : tags) {
        QTags.push_back(QString::fromStdString(tag));
    }
    bool ok = false;
    QString tag = QInputDialog::getItem(this, "Select tag to remove", "Tag: ", QTags, 0, false, &ok);
    if(ok && !tag.isEmpty()) {
        fileStruct.removeTag(ui->FileList->currentItem()->text().toStdString(), tag.toStdString());
        updateDisplay();
    }
}

void MainWindow::updateDisplay() {
    if(!ui->FileList->currentItem()) {
        return;
    }
    ui->TagList->clear();
    std::vector<std::string> tags = fileStruct.getTags(ui->FileList->currentItem()->text().toStdString());
    for(auto& tag : tags) {
        if(tag == "IsExist") {
            continue;
        }
        ui->TagList->addItem(QString::fromStdString(tag));
    }

    QPixmap img(ui->FileList->currentItem()->text());
    if (!ui->graphicsView->scene()) {
        qDebug() << "Creating scene";
        QGraphicsScene *scene = new QGraphicsScene(this);
        ui->graphicsView->setScene(scene);
    }
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->setSceneRect(0, 0, 0, 0);
    ui->graphicsView->scene()->addPixmap(img);
    ui->graphicsView->fitInView(ui->graphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    ui->graphicsView->centerOn(0, 0);
}

void MainWindow::saveIni() {
    if(!ui->FileList->count()) {
        QMessageBox msgBox;
        msgBox.setText("Nothing to save");
        msgBox.exec();
        return;
    }
    QString fileName =  QFileDialog::getSaveFileName(this, tr("Save Inifile"), ".", "Inifiles (*.ini)");
    if(fileName.isEmpty()) {
        return;
    }
    qDebug() << "Saved in: " << fileName;
    fileStruct.saveChanges(fileName.toStdString());
}

void MainWindow::showGroupDialog() {
    if(!ui->FileList->count()) {
        QMessageBox msgBox;
        msgBox.setText("Nothing to group");
        msgBox.exec();
        return;
    }
    QStringList QTags;
    std::vector<std::string> tags = fileStruct.getAllTags();
    for(auto& tag : tags) {
        QTags.push_back(QString::fromStdString(tag));
    }
    groupUi->setNewTags(QTags);
    groupUi->exec();
}

void MainWindow::groupFiles(std::vector<std::string> tags) {
    if(!tags.empty()) {
        fileStruct.groupFiles(tags);
    }
}

MainWindow::~MainWindow()
{
    delete groupUi;
    delete ui;
}

