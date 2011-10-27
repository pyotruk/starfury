#include "mainwindow.h"
#include "ui_mainwindow.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _img(new QImage(_defaultImgWidth, _defaultImgHeight, QImage::Format_RGB32))
{
    ui->setupUi(this);

    //gui <--> gui connections
    connect(ui->sliderStrobSize, SIGNAL(sliderMoved(int)),
            this, SIGNAL(changeStrobSize(int)));
    connect(ui->sliderStrobSize, SIGNAL(sliderMoved(int)),
            this, SLOT(updateFace()));
    connect(ui->sliderTrackingThreshold, SIGNAL(sliderMoved(int)),
            this, SIGNAL(changeTrackingThreshold(int)));
    connect(ui->sliderTrackingThreshold, SIGNAL(sliderMoved(int)),
            this, SLOT(updateFace()));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{
    delete _img;
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::initFace(const int strobSize,
                          const int trackTresh)
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
void MainWindow::drawFrame(const void *frame,
                           const int  frameWidth,
                           const int  frameHeight)
{
    setImgSize(QSize(frameWidth, frameHeight));
    uchar *ff = (uchar*)frame;
    QRgb *pLineStart, *pLineEnd;
    for(int j = 0; j < frameHeight; ++j)
    {
        pLineStart = (QRgb*)_img->scanLine(j);
        pLineEnd = pLineStart + frameWidth;
        for(QRgb *pline = pLineStart; pline < pLineEnd; ++pline)
        {
            *pline = qRgba(*ff, *ff, *ff, 255);
            ++ff;
        }
    }
    unlockSlowBuf(); //slowBuf unlocking (in rapidThread.doubleBuffer)
    update();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPoint p(0, 0);
    painter.drawImage(p, *_img);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::setImgSize(const QSize &frameSize)
{
    if(_img->size() != frameSize)
    {
        delete _img;
        _img = new QImage(frameSize, QImage::Format_RGB32);
        adaptWindowSize(frameSize);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::adaptWindowSize(const QSize &imgSize)
{
    QRect rect(this->geometry());
    rect.setHeight(imgSize.height() + ui->groupBoxTracking->geometry().height() + 40);
    rect.setWidth(imgSize.width());
    this->setGeometry(rect);
    rect = ui->groupBoxTracking->geometry();
    rect.moveTop(imgSize.height());
    ui->groupBoxTracking->setGeometry(rect);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
