#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "screenshot.h"
#include "googleapis.h"
#include "imgprocess.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isGettingPoint(false),
    nowWid(0)
{
    ui->setupUi(this);

    ui->shotView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->languageComboBox->setVisible(false);

    ui->languageComboBox->addItems(QStringList::fromVector(getSupportedLanguages()));
    ui->languageComboBox->setCurrentText("zh");

    ui->shotView->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshScreenshot()
{
    QPixmap screenshot = screenshotFromWId(nowWid);

    QString title = titleFromWId(nowWid);
    qDebug() << "title: " << title;

    if(ui->ocrCheckBox->checkState() == Qt::Checked)
    {
        OCRResult result = doOCR(screenshot);
        screenshot = DrawOCRBox(screenshot, result);
    }

    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(screenshot);
    item->setTransformationMode(Qt::SmoothTransformation);
    scene->addItem(item);


    ui->shotView->setScene(scene);
    ui->titleLabel->setText(title);

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = qobject_cast<QLabel*>(childAt(event->pos()));

    if(child == ui->pointLabel)
    {
        isGettingPoint = true;
        setCursor(QCursor(QPixmap(":/cursor/cross")));
        ui->pointLabel->setText("");
        ui->pointLabel->setEnabled(false);
    }
    else if(child == ui->refreshLabel)
    {
        ui->refreshLabel->setEnabled(false);
        ui->refreshLabel->setText("");
        refreshScreenshot();
        ui->refreshLabel->setEnabled(true);
        ui->refreshLabel->setText("<img src=':/icon/refresh'/>");
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(isGettingPoint)
    {
        setCursor(Qt::ArrowCursor);

        auto pos = event->globalPos();
        qDebug() << "select at " << pos;
        nowWid = WIdFromPoint(pos);

        refreshScreenshot();

        isGettingPoint = false;
        ui->pointLabel->setText("<img src=':/cursor/cross'/>");
        ui->pointLabel->setEnabled(true);
    }
}

void MainWindow::on_translateCheckBox_stateChanged(int arg)
{
    ui->languageComboBox->setVisible(arg);

    if(arg && ui->ocrCheckBox->checkState() == Qt::Unchecked)
    {
        ui->ocrCheckBox->setCheckState(Qt::Checked);
    }

}
