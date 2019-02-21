#ifndef GRAPHICSVIEWWITHOUTWHEEL_H
#define GRAPHICSVIEWWITHOUTWHEEL_H

#include <QGraphicsView>

class PhotoView : public QGraphicsView
{
    Q_OBJECT

public:

    PhotoView(QWidget *parent = Q_NULLPTR);
    PhotoView(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR);

    virtual ~PhotoView() {}

private:

    void wheelEvent(QWheelEvent *zoomEvent) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:

    void mousePressed(QPointF point);
    void mouseMoved(QPointF point);
    void mouseReleased(QPointF point);

};

#endif // GRAPHICSVIEWWITHOUTWHEEL_H
