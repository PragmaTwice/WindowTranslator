#include "photoview.h"
#include <QWheelEvent>

void PhotoView::wheelEvent(QWheelEvent * zoomEvent)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    static const double scaleFactor = 1.15;
    static double currentScale = 1.0;  // stores the current scale value.
    static const double scaleMin = .1; // defines the min scale limit.

    if(zoomEvent->delta() > 0) {
        scale(scaleFactor, scaleFactor);
        currentScale *= scaleFactor;
    } else if (currentScale > scaleMin) {
        scale(1 / scaleFactor, 1 / scaleFactor);
        currentScale /= scaleFactor;
    }
}

void PhotoView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}

void PhotoView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void PhotoView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
}
