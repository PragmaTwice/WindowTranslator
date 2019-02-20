#include "imgprocess.h"
#include <QPainter>

QPixmap DrawOCRBox(const QPixmap &origin, const OCRResult &ocrResult)
{
    QPixmap pixmap = origin;

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);

    painter.setPen(Qt::blue);
    for(auto&& box : ocrResult)
    {
        painter.drawPolygon(box.vertices);
    }

    return pixmap;

}
