#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QPlainTextEdit>

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
    connect(ui->OpenManAct, &QAction::triggered, this, &MainWindow::openManual);

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
    connect(ui->RotateRightButton, &QPushButton::clicked, ui->graphicsView, &ImageView::rotateRight);
    connect(ui->RotateLeftButton, &QPushButton::clicked, ui->graphicsView, &ImageView::rotateLeft);
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

void MainWindow::openManual() {
    QFile manFile(":/manual/Manual.txt");
    if(!manFile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", manFile.errorString());
        return;
    }
    QString manualText;
    QTextStream in(&manFile);
    in.setCodec("UTF-8");
    manualText = in.readAll();
    manFile.close();

    QDialog* manualUi = new QDialog(this);
    manualUi->setGeometry(this->geometry());
    QVBoxLayout* vertLayout = new QVBoxLayout(manualUi);
    manualUi->setMinimumHeight(100);

    QTextEdit* text = new QTextEdit(manualUi);
    text->setReadOnly(true);
    QString fontInfoStr = "[Font]:";
    int fontIndex = manualText.indexOf(fontInfoStr); // Example: "[Font]:16,400."
    while(fontIndex >= 0) {
        fontIndex += fontInfoStr.size();
        int valSize = manualText.indexOf(",", fontIndex) - fontIndex;
        int fontSize = manualText.midRef(fontIndex, valSize).toInt();
        fontIndex = manualText.indexOf(",", fontIndex) + 1;
        valSize = manualText.indexOf(".", fontIndex) - fontIndex;
        int fontWeight = manualText.midRef(fontIndex, valSize).toInt();
        text->setCurrentFont(QFont("sanf serif", fontSize, fontWeight, false));
        fontIndex = manualText.indexOf(".", fontIndex) + 1;
        valSize = manualText.indexOf(fontInfoStr, fontIndex) - fontIndex;
        text->insertPlainText(manualText.mid(fontIndex, valSize));
        fontIndex = manualText.indexOf(fontInfoStr, fontIndex);
    }
    vertLayout->addWidget(text);
    manualUi->setLayout(vertLayout);
    manualUi->exec();
    delete manualUi;
}

void MainWindow::showImageViewWindow() {
    QStringList files;
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

void MainWindow::openFolder() {
    QString folderName =  QFileDialog::getExistingDirectory(this, tr("Открыть папку"), "", QFileDialog::ShowDirsOnly);
    if(folderName.isEmpty()) {
        return;
    }
    isSaved = false;
    fileStruct.openNewFolder(folderName.toStdString());
    qDebug() << "Opening folder: " << folderName;
    updateFileList();
}

void MainWindow::addFolder() {
    QString folderName =  QFileDialog::getExistingDirectory(this, tr("Добавить папку"), "", QFileDialog::ShowDirsOnly);
    if(folderName.isEmpty()) {
        return;
    }
    isSaved = false;
    fileStruct.addFolder(folderName.toStdString());
    qDebug() << "Adding folder: " << folderName;
    updateFileList();
}

void MainWindow::openIni() {
    QString fileName =  QFileDialog::getOpenFileName(this, tr("Открыть файл с метками"), "", "Inifiles (*.ini)");
    if(fileName.isEmpty()) {
        return;
    }
    fileStruct.openData(fileName.toStdString());
    qDebug() << "Opening ini: " << fileName;
    updateFileList();
}

void MainWindow::addNewIni() {
    QString fileName =  QFileDialog::getOpenFileName(this, tr("Добавить файл с метками"), "", "Inifiles (*.ini)");
    if(fileName.isEmpty()) {
        return;
    }
    isSaved = false;
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
    QStringList QTags = fileStruct.getAllTags();
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
    QStringList Tags = fileStruct.getTags(ui->FileList->currentItem()->text().toStdString());
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
    QStringList QTags = fileStruct.getTags(ui->FileList->currentItem()->text().toStdString());
    for(auto& tag : QTags) {
        if(tag == "IsExist") {
            continue;
        }
        ui->TagList->addItem(tag);
    }
    ui->graphicsView->setImage(ui->FileList->currentItem()->text());
}

void MainWindow::updateFileList() {
    QStringList files;
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
        ui->FileList->addItem(file);
    }
}

void MainWindow::saveIni() {
    if(fileStruct.isEmpty()) {
        QMessageBox::information(this, "Сообщение", "Нечего сохранять");
        return;
    }
    QString fileName =  QFileDialog::getSaveFileName(this, tr("Сохранить файл с метками"), "", "Inifiles (*.ini)");
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
    groupUi->setNewTags(fileStruct.getAllTags());
    groupUi->exec();
}

void MainWindow::groupFiles(const QStringList& tags) {
    fileStruct.groupFiles(tags);
    updateFileList();
}

void MainWindow::copyGroup() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Копировать в папку"), "", QFileDialog::ShowDirsOnly);
    if(!path.isEmpty()) {
        fileStruct.saveToFolder(path.toStdString(), false);
        updateFileList();
    }
}

void MainWindow::moveGroup() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Переместить в папку"), "", QFileDialog::ShowDirsOnly);
    if(!path.isEmpty()) {
        fileStruct.saveToFolder(path.toStdString(), true);
        updateFileList();
    }
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

