#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    bool isGettingPoint;
    WId nowWid;

    void refreshScreenshot();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // MAINWINDOW_H
