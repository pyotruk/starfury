#include "mainwindow.h"
#include "ui_mainwindow.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    FImg = new QImage(640, 480, QImage::Format_RGB32);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete FImg;
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::DrawFrameSlot(void   *pFrame,
                               int frameWidth,
                               int frameHeight)
{
    uchar *ff = (uchar*)pFrame;
    QRgb *pline;
    for(int j = 0; j < frameHeight; j++)
    {
        pline = (QRgb*)(*FImg).scanLine(j);
        for(int i = 0; i < frameWidth; i++)
        {
            *pline = qRgba(*ff, *ff, *ff, 0xff);
            pline++;
            ff++;
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
