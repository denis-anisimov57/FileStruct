#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QAction>
#include "FileStruct.h"
#include "groupdialog.h"
#include "imageview.h"
#include "imageviewwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    virtual void closeEvent(QCloseEvent* event) override;

    ~MainWindow();

private:
    FileStruct fileStruct;
    bool isSaved = true;
    QAction* addTagAct;
    QAction* addExistingTagAct;
    QAction* removeTagAct;
    QMenu* filesContextMenu;
    QMenu* tagsContextMenu;
    ImageViewWindow* imageUi;
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
    void groupFiles(const QStringList& tags);
    void copyGroup();
    void moveGroup();
    void showGroupDialog();
    void showFilesContextMenu(QPoint);
    void showTagsContextMenu(QPoint);
    void showImageViewWindow();
    void openManual();
};

#endif // MAINWINDOW_H
