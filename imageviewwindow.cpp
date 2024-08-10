#include "imageviewwindow.h"
#include "ui_imageviewwindow.h"

ImageViewWindow::ImageViewWindow(QWidget *parent) :
    QWidget(parent),
    goLeftAct(new QAction()),
    goRightAct(new QAction()),
    ui(new Ui::ImageViewWindow)
{
    ui->setupUi(this);
    goLeftAct->setShortcut(QKeySequence("Q"));
    goRightAct->setShortcut(QKeySequence("E"));
    this->addAction(goLeftAct);
    this->addAction(goRightAct);
    connect(goLeftAct, &QAction::triggered, this, &ImageViewWindow::goLeft);
    connect(goRightAct, &QAction::triggered, this, &ImageViewWindow::goRight);

}

void ImageViewWindow::setImageList(QStringList fileList, int current) {
    imageList = fileList;
    currentImg = current;
    ui->graphicsView->setImage(imageList[current]);
}

void ImageViewWindow::goLeft() {
    if(currentImg > 0) {
        currentImg--;
        ui->graphicsView->setImage(imageList[currentImg]);
    }
}

void ImageViewWindow::goRight() {
    if(currentImg < imageList.size() - 1) {
        currentImg++;
        ui->graphicsView->setImage(imageList[currentImg]);
    }
}

ImageViewWindow::~ImageViewWindow()
{
    delete goLeftAct;
    delete goRightAct;
    delete ui;
}
