#include "mainwindow.h"
#include "ui_mainwindow.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
void MainWindow::drawFrame(Frame *frame)
{
    QSize frameSize(frame->header().width, frame->header().height);
    if(_img.size() != frameSize)
    {
        adaptWindowSize(frameSize);
    }
    _img = frame->asQImage();
    unlockSlowBuf(); //slowBuf unlocking (in rapidThread.doubleBuffer)
    update();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPoint p(0, 0);
    painter.drawImage(p, _img);
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
