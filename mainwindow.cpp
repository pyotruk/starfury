#include "mainwindow.h"
#include "ui_mainwindow.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //GUI connections
    connect(ui->sliderStrobSize, SIGNAL(sliderMoved(int)),
            this, SIGNAL(changeStrobSize(int)));
    connect(ui->sliderStrobSize, SIGNAL(sliderMoved(int)),
            this, SLOT(updateFace()));
    connect(ui->sliderTrackingThreshold, SIGNAL(sliderMoved(int)),
            this, SIGNAL(changeTrackingThreshold(int)));
    connect(ui->sliderTrackingThreshold, SIGNAL(sliderMoved(int)),
            this, SLOT(updateFace()));

    FImg = new QImage(640, 480, QImage::Format_RGB32); //magic numbers!!!
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete FImg;
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::initFace(int strobSize, int trackTresh)
{
    ui->sliderStrobSize->setValue(strobSize);
    ui->sliderTrackingThreshold->setValue(trackTresh);
    updateFace();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::updateFace()
{
    QVariant content;
    content = ui->sliderStrobSize->value();
    ui->labelStrobSize->setText("size " + content.toString());
    content = ui->sliderTrackingThreshold->value();
    ui->labelThreshold->setText("thr " + content.toString());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::drawFrame(void *pFrame,
                           int  frameWidth,
                           int  frameHeight)
{
    uchar *ff = (uchar*)pFrame;
    QRgb *pLineStart, *pLineEnd;
    for(int j = 0; j < frameHeight; ++j)
    {
        pLineStart = (QRgb*)(*FImg).scanLine(j);
        pLineEnd = pLineStart + frameWidth;
        for(QRgb *pline = pLineStart; pline < pLineEnd; ++pline)
        {
            *pline = qRgba(*ff, *ff, *ff, 255);
            ++ff;
        }
    }
    update();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPoint p(0, 0);
    painter.drawImage(p, *FImg);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
