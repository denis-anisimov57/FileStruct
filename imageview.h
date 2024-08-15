#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H
#include <QMainWindow>
#include <QGraphicsView>
#include <QWheelEvent>

class ImageView: public QGraphicsView {
    Q_OBJECT
public:
    ImageView(QWidget* parent);
    virtual void wheelEvent(QWheelEvent* event) override;
    void setImage(QString filePath);
    void rotateLeft();
    void rotateRight();
};

#endif // IMAGEVIEW_H
