#ifndef GRAPHICSVIEWWITHOUTWHEEL_H
#define GRAPHICSVIEWWITHOUTWHEEL_H

#include <QGraphicsView>

class PhotoView : public QGraphicsView
{
public:
    using QGraphicsView::QGraphicsView;

private:

    void wheelEvent(QWheelEvent *zoomEvent) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // GRAPHICSVIEWWITHOUTWHEEL_H
