#ifndef GRAPHICSVIEWWITHOUTWHEEL_H
#define GRAPHICSVIEWWITHOUTWHEEL_H

#include <QGraphicsView>

class GraphicsViewWithoutWheel : public QGraphicsView
{
public:
    using QGraphicsView::QGraphicsView;

private:

    void wheelEvent(QWheelEvent *ZoomEvent) override;
};

#endif // GRAPHICSVIEWWITHOUTWHEEL_H
