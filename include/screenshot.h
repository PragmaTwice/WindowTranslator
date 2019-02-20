#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QPoint>
#include <QPixmap>

WId WIdFromPoint(QPoint point);

QPixmap screenshotFromWId(WId wid);

QString titleFromWId(WId wid);

#endif // SCREENSHOT_H
