#ifndef GOOGLEAPIS_H
#define GOOGLEAPIS_H

#include <QPixmap>
#include <QVector>

struct OCRBox
{
    QString description;
    QVector<QPointF> vertices;
};

QDebug operator<<(QDebug dbg, const OCRBox &box);

using OCRResult = QVector<OCRBox>;

QString getKey();

OCRResult doOCR(const QPixmap& pixmap);

#endif // GOOGLEAPIS_H
