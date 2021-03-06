#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "screenshot.h"
#include "googleapis.h"
#include "imgprocess.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isGettingPoint(false),
    isLoading(false),
    nowWid(0)
{
    ui->setupUi(this);

    ui->shotView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->languageComboBox->setVisible(false);

    ui->languageComboBox->addItems(QStringList::fromVector(getSupportedLanguages()));
    ui->languageComboBox->setCurrentText("zh");

    ui->shotView->setMouseTracking(true);
    connect(ui->shotView, SIGNAL(mousePressed(QPointF)), SLOT(on_shotView_mousePressed(QPointF)));
    connect(ui->shotView, SIGNAL(mouseMoved(QPointF)), SLOT(on_shotView_mouseMoved(QPointF)));

    item.setTransformationMode(Qt::SmoothTransformation);
    scene.addItem(&item);

    ui->translateLabel->setVisible(false);
    ui->translateCheckBox->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshScreenshot()
{
    isLoading = true;
    QPixmap screenshot = screenshotFromWId(nowWid);
    nowShot = screenshot;
    nowOCRRes = OCRResult();

    QString title = titleFromWId(nowWid);
    qDebug() << "title: " << title;

    if(ui->ocrCheckBox->checkState() == Qt::Checked)
    {
        nowOCRRes = doOCR(screenshot);
        screenshot = DrawOCRBox(screenshot, nowOCRRes);

        if(ui->translateCheckBox->checkState() == Qt::Checked)
        {
            nowTranslateMap = translateOCRResult(nowOCRRes, ui->languageComboBox->currentText());
        }
    }

    item.setPixmap(screenshot);

    nowTitle = title;

    ui->shotView->setScene(&scene);
    ui->titleLabel->setText(nowTitle);

    isLoading = false;

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

void MainWindow::on_shotView_mouseMoved(QPointF point)
{
    if(!isLoading)
    {
        static bool isActive = false;
        if(ui->ocrCheckBox->checkState() == Qt::Checked)
        {
            OCRBox res;
            bool isFirst = true;
            for(auto&& box : nowOCRRes)
            {
                if(isFirst)
                {
                    isFirst = false;
                    continue;
                }
                if(QPolygonF(box.vertices).containsPoint(point, Qt::OddEvenFill))
                {
                    res = box;
                    break;
                }
            }
            if(!res.description.isEmpty())
            {
                isActive = true;
                item.setPixmap(DrawOCRBox(nowShot, nowOCRRes, &res));
                ui->titleLabel->setText(res.description);
                if(ui->translateCheckBox->checkState() == Qt::Checked)
                {
                    auto translate = nowTranslateMap.find(res.description);
                    if(translate != nowTranslateMap.cend())
                    {
                        ui->translateLabel->setVisible(true);
                        ui->translateLabel->setText(translate.value());
                    }
                }
            }
            else
            {
                if(isActive)
                {
                    item.setPixmap(DrawOCRBox(nowShot, nowOCRRes));
                    isActive = false;
                }
                ui->titleLabel->setText(nowTitle);
                ui->translateLabel->setVisible(false);
            }
        }
    }
}

void MainWindow::on_shotView_mousePressed(QPointF)
{
    if(!isLoading)
    {
        QApplication::clipboard()->setText(ui->titleLabel->text());
    }
}

void MainWindow::on_ocrCheckBox_stateChanged(int arg)
{
    if(arg == 0)
    {
        ui->translateCheckBox->setChecked(false);
        ui->translateCheckBox->setVisible(false);
    }
    else
    {
        ui->translateCheckBox->setVisible(true);
    }
}
