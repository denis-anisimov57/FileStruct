#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QVector>
#include <QWheelEvent>
#include <QDialogButtonBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , fileStruct()
    , addTagAct(new QAction("Добавить метку"))
    , addExistingTagAct(new QAction("Добавить существующую метку"))
    , removeTagAct(new QAction("Удалить метку"))
    , filesContextMenu(new QMenu())
    , tagsContextMenu(new QMenu())
    , imageUi(new ImageViewWindow)
    , groupUi(new GroupDialog)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->AllButton->setChecked(true);

    groupUi->setWindowTitle("Фильтрация по меткам");
    imageUi->setWindowTitle("Просмотр файла");

    // add shortcuts?
    connect(ui->OpenFolderAct, &QAction::triggered, this, &MainWindow::openFolder);
    connect(ui->OpenTagsAct, &QAction::triggered, this, &MainWindow::openIni);
    connect(ui->AddFolderAct, &QAction::triggered, this, &MainWindow::addFolder);
    connect(ui->AddTagsAct, &QAction::triggered, this, &MainWindow::addNewIni);
    connect(ui->SaveTagsAct, &QAction::triggered, this, &MainWindow::saveIni);
    connect(ui->FilterFilesAct, &QAction::triggered, this, &MainWindow::showGroupDialog);
    connect(ui->CopyFilteredAct, &QAction::triggered, this, &MainWindow::copyGroup);
    connect(ui->MoveFilteredAct, &QAction::triggered, this, &MainWindow::moveGroup);

    ui->RotateRightButton->setText("");
    ui->RotateLeftButton->setText("");
    ui->RotateRightButton->setIcon(QIcon(":/icons/rotate-right.png"));
    ui->RotateLeftButton->setIcon(QIcon(":/icons/rotate-left.png"));

    ui->FileList->setContextMenuPolicy(Qt::CustomContextMenu);
    addTagAct->setShortcut(QKeySequence("Ctrl+E"));
    addExistingTagAct->setShortcut(QKeySequence("Ctrl+Shift+E"));
    filesContextMenu->addAction(addTagAct);
    filesContextMenu->addAction(addExistingTagAct);
    this->addAction(addTagAct);
    this->addAction(addExistingTagAct);

    ui->TagList->setContextMenuPolicy(Qt::CustomContextMenu);
    removeTagAct->setShortcut(QKeySequence("Ctrl+D"));
    tagsContextMenu->addAction(removeTagAct);
    this->addAction(removeTagAct);

    connect(addTagAct, &QAction::triggered, this, &MainWindow::addTag);
    connect(addExistingTagAct, &QAction::triggered, this, &MainWindow::addExistingTag);
    connect(ui->FileList, &QListWidget::currentRowChanged, this, &MainWindow::updateDisplay);
    connect(ui->FileList, &QListWidget::itemDoubleClicked, this, &MainWindow::showImageViewWindow);
    connect(removeTagAct, &QAction::triggered, this, &MainWindow::removeTag);
    connect(groupUi, &GroupDialog::dialogRes, this, &MainWindow::groupFiles);
    connect(ui->AllButton, &QRadioButton::clicked, this, &MainWindow::updateFileList);
    connect(ui->FilteredButton, &QRadioButton::clicked, this, &MainWindow::updateFileList);
    connect(ui->TaggedButton, &QRadioButton::clicked, this, &MainWindow::updateFileList);
    connect(ui->UntaggedButton, &QRadioButton::clicked, this, &MainWindow::updateFileList);
    connect(ui->RotateRightButton, &QPushButton::clicked, this, &MainWindow::rotateRight);
    connect(ui->RotateLeftButton, &QPushButton::clicked, this, &MainWindow::rotateLeft);
    connect(ui->FileList, &QWidget::customContextMenuRequested, this, &MainWindow::showFilesContextMenu);
    connect(ui->TagList, &QWidget::customContextMenuRequested, this, &MainWindow::showTagsContextMenu);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if(!isSaved) {
        event->ignore();
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle("Подтверждение выхода");
        msgBox.setText("Выйти без сохранения?");
        QPushButton* yesButton = new QPushButton("Да", this);
        QPushButton* noButton = new QPushButton("Нет", this);
        msgBox.addButton(yesButton, QMessageBox::AcceptRole);
        msgBox.addButton(noButton, QMessageBox::RejectRole);
        int res = msgBox.exec();
        if (res == QMessageBox::AcceptRole) {
            delete yesButton;
            delete noButton;
            event->accept();
        }
    }
}

void MainWindow::showImageViewWindow() {
    QStringList files;
    // store items in class?
    QList<QListWidgetItem*> items = ui->FileList->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);

    for(auto& item : items) {
        files.push_back(item->text());
    }
    imageUi->show();

    imageUi->setImageList(files, ui->FileList->currentRow());
}

void MainWindow::showFilesContextMenu(QPoint pos) {
    QPoint globalPos = ui->FileList->viewport()->mapToGlobal(pos);
    filesContextMenu->exec(globalPos);
}

void MainWindow::showTagsContextMenu(QPoint pos) {
    QPoint globalPos = ui->TagList->viewport()->mapToGlobal(pos);
    tagsContextMenu->exec(globalPos);
}

// make openFolder and AddFolder in one method?
void MainWindow::openFolder() {
    QString folderName =  QFileDialog::getExistingDirectory(this, tr("Открыть папку"), ".", QFileDialog::ShowDirsOnly);
    if(folderName.isEmpty()) {
        return;
    }
    isSaved = false;
//    ui->TagList->clear();
//    ui->FileList->clear();
    fileStruct.openNewFolder(folderName.toStdString());
    qDebug() << "Opening folder: " << folderName;
    updateFileList();
}

void MainWindow::addFolder() {
    QString folderName =  QFileDialog::getExistingDirectory(this, tr("Добавить папку"), ".", QFileDialog::ShowDirsOnly);
    if(folderName.isEmpty()) {
        return;
    }
    isSaved = false;
    std::vector<std::string> newFiles = fileStruct.addFolder(folderName.toStdString());
    qDebug() << "Adding folder: " << folderName;
    updateFileList();
}

//make openIni and AddIni in one method or function?
void MainWindow::openIni() {
    QString fileName =  QFileDialog::getOpenFileName(this, tr("Открыть файл с метками"), ".", "Inifiles (*.ini)");
    if(fileName.isEmpty()) {
        return;
    }
//    ui->TagList->clear();
//    ui->FileList->clear();
    fileStruct.openData(fileName.toStdString());
    qDebug() << "Opening ini: " << fileName;
    updateFileList();
}

void MainWindow::addNewIni() {
    QString fileName =  QFileDialog::getOpenFileName(this, tr("Добавить файл с метками"), ".", "Inifiles (*.ini)");
    if(fileName.isEmpty()) {
        return;
    }
    isSaved = false;
//    ui->TagList->clear();
//    ui->FileList->clear();
    fileStruct.addData(fileName.toStdString());
    qDebug() << "Adding ini: " << fileName;
    updateFileList();
}

void MainWindow::addTag() {
    if(ui->FileList->selectedItems().empty()) {
        QMessageBox::information(this, "Сообщение", "Не выбран файл");
        return;
    }
    bool ok = false;
    QString tag = QInputDialog::getText(this, tr("Добавить метку"), tr("Введите метку"), QLineEdit::Normal, "", &ok);
    if(ok && !tag.isEmpty()) {
        fileStruct.addTag(ui->FileList->currentItem()->text().toStdString(), tag.toStdString());
        if(ui->UntaggedButton->isChecked()) {
            ui->FileList->currentItem()->setBackgroundColor(QColor(0, 255, 255));
        }
        isSaved = false;
        updateDisplay();
    }
}

void MainWindow::addExistingTag() {
    if(ui->FileList->selectedItems().empty()) {
        QMessageBox::information(this, "Сообщение", "Не выбран файл");
        return;
    }
    fileStruct.removeUnusedTags();
    // use QString instead of std::string?
    std::vector<std::string> tags = fileStruct.getAllTags();
    QStringList QTags;
    for(auto& tag : tags) {
        QTags.push_back(QString::fromStdString(tag));
    }
    bool ok = false;
    QString tag = QInputDialog::getItem(this, "Выберите метки для фильтрации", "Метка: ", QTags, 0, false, &ok);
    if(ok && !tag.isEmpty()) {
        fileStruct.addTag(ui->FileList->currentItem()->text().toStdString(), tag.toStdString());
        if(ui->UntaggedButton->isChecked()) {
            ui->FileList->currentItem()->setBackgroundColor(QColor(0, 255, 255));
        }
        isSaved = false;
        updateDisplay();
    }
}

void MainWindow::removeTag() {
    if(ui->FileList->selectedItems().empty()) {
        QMessageBox::information(this, "Сообщение", "Не выбран файл");
        return;
    }
    if(ui->TagList->selectedItems().empty()) {
        QMessageBox::information(this, "Сообщение", "Не выбрана метка");
        return;
    }
    // use QString instead of std::string? QStringList?
    QStringList QTags;
    std::vector<std::string> tags = fileStruct.getTags(ui->FileList->currentItem()->text().toStdString());
    for(auto& tag : tags) {
        QTags.push_back(QString::fromStdString(tag));
    }
    QString tag = ui->TagList->currentItem()->text();
    fileStruct.removeTag(ui->FileList->currentItem()->text().toStdString(), tag.toStdString());
    isSaved = false;
    updateDisplay();
    if(ui->TaggedButton->isChecked() && !ui->TagList->count()) {
        ui->FileList->currentItem()->setBackgroundColor(QColor(0, 255, 255));
    }
}

void MainWindow::updateDisplay() {
    if(!ui->FileList->currentItem()) {
        return;
    }
    ui->TagList->clear();
    // use QString instead of std::string?
    std::vector<std::string> tags = fileStruct.getTags(ui->FileList->currentItem()->text().toStdString());
    for(auto& tag : tags) {
        if(tag == "IsExist") {
            continue;
        }
        ui->TagList->addItem(QString::fromStdString(tag));
    }
    ui->graphicsView->setImage(ui->FileList->currentItem()->text());
}

void MainWindow::updateFileList() {
    std::vector<std::string> files;
    // store all this arrays in the class?
    if(ui->UntaggedButton->isChecked()) {
        files = fileStruct.getUntagged();
    }
    else if(ui->TaggedButton->isChecked()) {
        files = fileStruct.getTagged();
    }
    else if(ui->FilteredButton->isChecked()) {
        files = fileStruct.getFiltered();
    }
    else {
        files = fileStruct.getFiles();
    }
    ui->FileList->clear();
    ui->TagList->clear();
    for(auto& file : files) {
        ui->FileList->addItem(QString::fromStdString(file));
    }
}

void MainWindow::saveIni() {
    if(fileStruct.isEmpty()) {
        QMessageBox::information(this, "Сообщение", "Нечего сохранять");
        return;
    }
    QString fileName =  QFileDialog::getSaveFileName(this, tr("Сохранить файл с метками"), ".", "Inifiles (*.ini)");
    if(fileName.isEmpty()) {
        return;
    }
    qDebug() << "Saved in: " << fileName;
    fileStruct.saveChanges(fileName.toStdString());
    isSaved = true;
}

void MainWindow::showGroupDialog() {
    if(fileStruct.isEmpty()) {
        QMessageBox::information(this, "Сообщение", "Нечего фильтровать");
        return;
    }
//     use QString?
    QStringList QTags;
    std::vector<std::string> tags = fileStruct.getAllTags();
    for(auto& tag : tags) {
        QTags.push_back(QString::fromStdString(tag));
    }
    groupUi->setNewTags(QTags);
    groupUi->exec();
}

void MainWindow::groupFiles(const std::vector<std::string>& tags) {
    fileStruct.groupFiles(tags);
    updateFileList();
}

// make copy and move in one method?
void MainWindow::copyGroup() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Копировать в папку"), ".", QFileDialog::ShowDirsOnly);
    if(!path.isEmpty()) {
        fileStruct.saveToFolder(path.toStdString(), false);
        updateFileList();
    }
}

void MainWindow::moveGroup() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Переместить в папку"), ".", QFileDialog::ShowDirsOnly);
    if(!path.isEmpty()) {
        fileStruct.saveToFolder(path.toStdString(), true);
        updateFileList();
    }
}

// make left and right rotation in one method?
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
    delete addTagAct;
    delete addExistingTagAct;
    delete removeTagAct;
    delete filesContextMenu;
    delete tagsContextMenu;
    delete imageUi;
    delete groupUi;
    delete ui;
}

