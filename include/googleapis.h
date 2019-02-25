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

QVector<QString> getSupportedLanguages();

QVector<QString> doTranslate(const QVector<QString>& text, const QString& targetLang);

QMap<QString, QString> translateOCRResult(const OCRResult& ocr, const QString &targetLang);

#endif // GOOGLEAPIS_H
