#include "screenshot.h"
#include <qt_windows.h>
#include <QScreen>
#include <QGuiApplication>
#include <QtWinExtras/QtWin>

WId WIdFromPoint(QPoint point)
{
    return (WId)WindowFromPoint(POINT{point.x(), point.y()});
}

QPixmap screenshotFromWId(WId wid)
{
//    static QScreen *screen = QGuiApplication::primaryScreen();
//    return screen->grabWindow(wid);

    HWND hwnd = (HWND)wid;
    RECT rc;
    GetClientRect(hwnd, &rc);

    //create
    HDC hdcScreen = GetDC(NULL);
    HDC hdc = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen,
        rc.right - rc.left, rc.bottom - rc.top);
    SelectObject(hdc, hbmp);

    //Print to memory hdc
    PrintWindow(hwnd, hdc, 0x00000002);

    //use
    QPixmap pixmap = QtWin::fromHBITMAP(hbmp);

    //release
    DeleteDC(hdc);
    DeleteObject(hbmp);
    ReleaseDC(NULL, hdcScreen);

    return pixmap;
}

QString titleFromWId(WId wid)
{
    static char buffer[1024];

    GetWindowTextA((HWND)wid, buffer, sizeof buffer);

    return QString::fromLocal8Bit(buffer).toUtf8();
}
