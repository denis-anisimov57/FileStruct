#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QAction>
#include "FileStruct.h"
#include "groupdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    FileStruct fileStruct;
    QStringList qFilteredFiles;
    QAction* addTagAct;
    QAction* addExistingTagAct;
    QAction* removeTagAct;
    QMenu* filesContextMenu;
    QMenu* tagsContextMenu;
    GroupDialog* groupUi;
    Ui::MainWindow *ui;
private slots:
    void openFolder();
    void addFolder();
    void openIni();
    void addNewIni();
    void addTag();
    void addExistingTag();
    void removeTag();
    void updateDisplay();
    void updateFileList();
    void saveIni();
    void groupFiles(const std::vector<std::string>& tags);
    void saveGroup();
    void showGroupDialog();
    void rotateRight();
    void rotateLeft();
    void showFilesContextMenu(QPoint);
    void showTagsContextMenu(QPoint);
};

class ImageView: public QGraphicsView {
    Q_OBJECT
public:
    ImageView(QWidget* parent);
    virtual void wheelEvent(QWheelEvent* event) override;
};

#endif // MAINWINDOW_H
