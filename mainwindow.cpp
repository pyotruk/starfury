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
    ArtifactVector::iterator it = _artifacts.begin();
    for(; it != _artifacts.end(); ++it)
    {
        drawCross(img,
                  it->center(),
                  _crossSide,
                  Qt::green);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::markStars(QImage &img)
{
    ArtifactVector::iterator it = _stars.begin();
    for(; it != _stars.end(); ++it)
    {
        drawCross(img,
                  it->center(),
                  (int)it->magnitude(),
                  Qt::magenta);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::drawFrame(Frame *frame,
                           QMutex *mutex)
{
    if(mutex->tryLock(_timeout))
    {
        _img = frame->asQImage();
        mutex->unlock();
    }
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
void MainWindow::fromEquator(ArtifactVector *artifacts,
                             ArtifactVector *stars,
                             QMutex *mutex)
{
    if(mutex->tryLock(_timeout))
    {
        _artifacts.resize(artifacts->size());
        _stars.resize(stars->size());
        qCopy(artifacts->begin(), artifacts->end(), _artifacts.begin());
        qCopy(stars->begin(), stars->end(), _stars.begin());
        mutex->unlock();
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


