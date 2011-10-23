#include "mainwindow.h"
#include "ui_mainwindow.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->scrollStrobSize, SIGNAL(sliderMoved(int)), this, SIGNAL(StrobSizeChangedSignal(int)));
    FImg = new QImage(640, 480, QImage::Format_RGB32);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete FImg;
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::InitFace(int strobSize)
{
    ui->scrollStrobSize->setValue(strobSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::DrawFrameSlot(void   *pFrame,
                               int frameWidth,
                               int frameHeight)
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
