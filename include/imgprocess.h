#ifndef IMGPROCESS_H
#define IMGPROCESS_H

#include "googleapis.h"

QPixmap DrawOCRBox(const QPixmap& origin, const OCRResult& ocrResult, const OCRBox *activeBox = nullptr);

#endif // IMGPROCESS_H
