#ifndef IMAGEVIEWWINDOW_H
#define IMAGEVIEWWINDOW_H

#include <QWidget>
#include "imageview.h"
#include <QAction>

namespace Ui {
class ImageViewWindow;
}

class ImageViewWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewWindow(QWidget *parent = nullptr);

    void setImageList(QStringList fileList, int current);

    ~ImageViewWindow();

private:
    QAction* goLeftAct;
    QAction* goRightAct;
    QStringList imageList;
    int currentImg = 0;
    Ui::ImageViewWindow *ui;
private slots:
    void goLeft();
    void goRight();
};

#endif // IMAGEVIEWWINDOW_H
