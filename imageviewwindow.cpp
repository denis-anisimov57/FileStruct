#include "imageviewwindow.h"
#include "ui_imageviewwindow.h"

ImageViewWindow::ImageViewWindow(QWidget *parent) :
    QWidget(parent),
    goLeftAct(new QAction()),
    goRightAct(new QAction()),
    ui(new Ui::ImageViewWindow)
{
    ui->setupUi(this);

    ui->RotateRightButton->setText("");
    ui->RotateLeftButton->setText("");
    ui->RotateRightButton->setIcon(QIcon(":/icons/rotate-right.png"));
    ui->RotateLeftButton->setIcon(QIcon(":/icons/rotate-left.png"));

    goLeftAct->setShortcut(QKeySequence(Qt::Key_Left));
    goRightAct->setShortcut(QKeySequence(Qt::Key_Right));
    this->addAction(goLeftAct);
    this->addAction(goRightAct);

    connect(goLeftAct, &QAction::triggered, this, &ImageViewWindow::goLeft);
    connect(goRightAct, &QAction::triggered, this, &ImageViewWindow::goRight);
    connect(ui->RotateRightButton, &QPushButton::clicked, ui->graphicsView, &ImageView::rotateRight);
    connect(ui->RotateLeftButton, &QPushButton::clicked, ui->graphicsView, &ImageView::rotateLeft);

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
