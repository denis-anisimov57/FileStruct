#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QVector>
#include <QWheelEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , fileStruct()
    , groupUi(new GroupDialog)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->AllButton->setChecked(true);
    connect(ui->OpenFolderButton, &QPushButton::clicked, this, &MainWindow::openFolder);
    connect(ui->AddButton, &QPushButton::clicked, this, &MainWindow::addTag);
    connect(ui->FileList, &QListWidget::currentRowChanged, this, &MainWindow::updateDisplay);
    connect(ui->OpenIniButton, &QPushButton::clicked, this, &MainWindow::openIni);
    connect(ui->SaveIniButton, &QPushButton::clicked, this, &MainWindow::saveIni);
    connect(ui->GroupButton, &QPushButton::clicked, this, &MainWindow::showGroupDialog);
    connect(ui->AddExistingButton, &QPushButton::clicked, this, &MainWindow::addExistingTag);
    connect(ui->RemoveButton, &QPushButton::clicked, this, &MainWindow::removeTag);
    connect(groupUi, &GroupDialog::dialogRes, this, &MainWindow::groupFiles);
    connect(ui->SaveGroupButton, &QPushButton::clicked, this, &MainWindow::saveGroup);
    connect(ui->UpdateButton, &QPushButton::clicked, this, &MainWindow::addFolder);
    connect(ui->AddIniButton, &QPushButton::clicked, this, &MainWindow::addNewIni);
    connect(ui->AllButton, &QRadioButton::clicked, this, &MainWindow::updateFileList);
    connect(ui->TaggedButton, &QRadioButton::clicked, this, &MainWindow::updateFileList);
    connect(ui->UntaggedButton, &QRadioButton::clicked, this, &MainWindow::updateFileList);
    connect(ui->RotateRightButton, &QPushButton::clicked, this, &MainWindow::rotateRight);
    connect(ui->RotateLeftButton, &QPushButton::clicked, this, &MainWindow::rotateLeft);
}

ImageView::ImageView(QWidget* parent): QGraphicsView(parent) {}

void ImageView::wheelEvent(QWheelEvent *event) {
    const double scaleFactor = 1.15;
    if(event->delta() > 0) {
        scale(scaleFactor, scaleFactor);
    }
    else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    qDebug() << event->key();
    if(event->key() == Qt::ArrowType::RightArrow) {
        qDebug() << "Right";
        ui->FileList->setCurrentRow(ui->FileList->currentRow() + 1);
    }
    else if(event->key() == Qt::ArrowType::LeftArrow) {
        qDebug() << "Left";
    }
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
    updateFileList();
}

void MainWindow::addFolder() {
    QString folderName =  QFileDialog::getExistingDirectory(this, tr("Open Directory"), ".", QFileDialog::ShowDirsOnly);
    std::vector<std::string> newFiles = fileStruct.addFolder(folderName.toStdString());
    updateFileList();
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
    updateFileList();
}

void MainWindow::addNewIni() {
    QString fileName =  QFileDialog::getOpenFileName(this, tr("Open Inifile"), ".", "Inifiles (*.ini)");
    if(fileName.isEmpty()) {
        return;
    }
    ui->TagList->clear();
    ui->FileList->clear();
    fileStruct.addData(fileName.toStdString());
    qDebug() << "Adding ini: " << fileName;
    updateFileList();
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
        if(ui->UntaggedButton->isChecked()) {
            ui->FileList->currentItem()->setBackgroundColor(QColor(0, 255, 255));
        }
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
    fileStruct.removeUnusedTags();
    QStringList QTags;
    std::vector<std::string> tags = fileStruct.getAllTags();
    for(auto& tag : tags) {
        QTags.push_back(QString::fromStdString(tag));
    }
    bool ok = false;
    QString tag = QInputDialog::getItem(this, "Select tag to combine", "Tag: ", QTags, 0, false, &ok);
    if(ok && !tag.isEmpty()) {
        fileStruct.addTag(ui->FileList->currentItem()->text().toStdString(), tag.toStdString());
        if(ui->UntaggedButton->isChecked()) {
            ui->FileList->currentItem()->setBackgroundColor(QColor(0, 255, 255));
        }
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
        if(ui->TaggedButton->isChecked() && !ui->TagList->count()) {
            ui->FileList->currentItem()->setBackgroundColor(QColor(0, 255, 255));
        }
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
    qDebug() << "Setting scene";
    if (ui->graphicsView->scene()) {
        delete ui->graphicsView->scene();
    }
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->scene()->addPixmap(img);
    ui->graphicsView->fitInView(ui->graphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    ui->graphicsView->centerOn(0, 0);
}

void MainWindow::updateFileList() {
    std::vector<std::string> files;
    if(ui->UntaggedButton->isChecked()) {
        files = fileStruct.getUntagged();
    }
    else if(ui->TaggedButton->isChecked()) {
        files = fileStruct.getTagged();
    }
    else {
        files = fileStruct.getFiles();
    }
    ui->FileList->clear();
    for(auto& file : files) {
        ui->FileList->addItem(QString::fromStdString(file));
    }
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

void MainWindow::groupFiles(const std::vector<std::string>& tags) {
    if(!tags.empty()) {
        fileStruct.groupFiles(tags);
    }
}

void MainWindow::saveGroup() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Save Directory"), ".", QFileDialog::ShowDirsOnly);
    if(!path.isEmpty()) {
        fileStruct.saveToFolder(path.toStdString());
    }
}

void MainWindow::rotateRight() {
    if(!ui->graphicsView->scene()) {
        return;
    }
    ui->graphicsView->rotate(90);
    ui->graphicsView->fitInView(ui->graphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::rotateLeft() {
    if(!ui->graphicsView->scene()) {
        return;
    }
    ui->graphicsView->rotate(-90);
    ui->graphicsView->fitInView(ui->graphicsView->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

MainWindow::~MainWindow()
{
    delete groupUi;
    delete ui;
}

