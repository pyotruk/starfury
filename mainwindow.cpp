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
void MainWindow::markArtifacts(QImage &img)
{
    ArtifactVector::iterator it = _artifactBox.artifacts().begin();
    for(; it != _artifactBox.artifacts().end(); ++it)
    {
        drawCrossbuck(img,
                      it->center(),
                      (int)it->magnitude(),
                      Qt::green);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::markStars(QImage &img)
{
    ArtifactVector::iterator it = _starBox.artifacts().begin();
    for(; it != _starBox.artifacts().end(); ++it)
    {
        drawCross(img,
                  it->center(),
                  (int)it->magnitude(),
                  Qt::magenta);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::drawAll()
{
    if(_img.size() != _imgSize)
    {
        adaptWindowSize(_img.size());
        _imgSize = _img.size();
        emit screenSizeChanged(_imgSize.width(), _imgSize.height());
    }
    this->markArtifacts(_img);
    this->markStars(_img);
    update();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::inputFrame(Frame *f)
{
    f->lock().lockForRead();
    f->copyToQImage(_img);
    f->lock().unlock();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::inputScreenStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _artifactBox = *a;
    a->lock().unlock();
    this->drawAll(); //<---------!!!
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::inputCatStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _starBox = *a;
    a->lock().unlock();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


