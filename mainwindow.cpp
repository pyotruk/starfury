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
void MainWindow::checkImgSize()
{
    if(_img.size() != _imgSize)
    {
        adaptWindowSize(_img.size());
        _imgSize = _img.size();
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
void MainWindow::markArtifacts(QImage &img,
                               ArtifactVector &a)
{
    ArtifactVector::iterator it = a.begin();
    for(; it != a.end(); ++it)
    {
        draw::crossbuck(img,
                        it->center(),
                        (int)it->magnitude(),
                        Qt::green);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::markStars(QImage &img,
                           ArtifactVector &s)
{
    ArtifactVector::iterator it = s.begin();
    for(; it != s.end(); ++it)
    {
        draw::cross(img,
                    it->center(),
                    (int)it->magnitude(),
                    Qt::magenta);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::drawTriangles(TriangleVector &tv,
                               const int width,
                               const QColor &color)
{
    foreach(ArtifactTriangle t, tv)
    {
        draw::triangle(_img,
                       width,
                       color,
                       t.t()[0].center(),
                       t.t()[1].center(),
                       t.t()[2].center());
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::drawAll(TriangleBoxData &d,
                         ArtifactVector &picStars,
                         ArtifactVector &catStars)
{
    this->checkImgSize();
    this->markArtifacts(_img, picStars);
    this->markStars(_img, catStars);
    this->drawTriangles(d.picTriangles,
                        2,
                        Qt::yellow);
    this->drawTriangles(d.catTriangles,
                        1,
                        Qt::blue);
    update();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::inputFrame(Frame *f)
{
    f->lock().lockForRead();
    f->copyToQImage(_img);
    f->lock().unlock();

    QTime time;
    time.start();
    this->drawAll(_tribox.data(),
                  _artifactBox.data(),
                  _starBox.data());
    qDebug() << "drawing delay: " << time.elapsed();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::inputScreenStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _artifactBox = *a;
    a->lock().unlock();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::inputCatStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _starBox = *a;
    a->lock().unlock();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::inputTriangles(TriangleBox *t)
{
    t->lock().lockForRead();
    _tribox = *t;
    t->lock().unlock();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


