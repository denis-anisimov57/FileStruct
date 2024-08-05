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
    void addFolder();
    void openIni();
    void addNewIni();
    void addTag();
    void addExistingTag();
    void removeTag();
    void updateDisplay();
    void saveIni();
    void groupFiles(const std::vector<std::string>& tags);
    void saveGroup();
    void showGroupDialog();
};
#endif // MAINWINDOW_H
