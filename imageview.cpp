#include "imageview.h"
#include <QDebug>

ImageView::ImageView(QWidget* parent): QGraphicsView(parent) {
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void ImageView::wheelEvent(QWheelEvent *event) {
    const double scaleFactor = 1.15;
    if(event->delta() > 0) {
        scale(scaleFactor, scaleFactor);
    }
    else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void ImageView::setImage(QString filePath) {
    QPixmap img(filePath);
    qDebug() << "Setting scene";
    if (this->scene()) {
        delete this->scene();
    }
    QGraphicsScene *scene = new QGraphicsScene(this);
    setScene(scene);
    this->scene()->addPixmap(img);
    fitInView(this->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
    centerOn(0, 0);
}
