#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    GroupDialog* groupUi;
    Ui::MainWindow *ui;
private slots:
    void openFolder();
    void openIni();
    void addTag();
    void addExistingTag();
    void removeTag();
    void updateTags();
    void saveIni();
    void combineFiles(std::vector<std::string> tags);
    void showGroupDialog();
};
#endif // MAINWINDOW_H
