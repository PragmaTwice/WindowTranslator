#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "googleapis.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_translateCheckBox_stateChanged(int arg1);

    void on_shotView_mouseMoved(QPointF point);
    void on_shotView_mousePressed(QPointF);

private:
    Ui::MainWindow *ui;
    bool isGettingPoint;

    WId nowWid;
    QString nowTitle;
    QPixmap nowShot;
    OCRResult nowOCRRes;

    QMap<QString, QString> nowTranslateMap;

    QGraphicsScene scene;
    QGraphicsPixmapItem item;

    void refreshScreenshot();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // MAINWINDOW_H
