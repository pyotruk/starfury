#include "imagewindow.h"
/////////////////////////////////////////////////////////////////////////////////////
ImageWindow::ImageWindow(QWidget *parent) :
    QMainWindow(parent)
{}
/////////////////////////////////////////////////////////////////////////////////////
ImageWindow::~ImageWindow()
{}
/////////////////////////////////////////////////////////////////////////////////////
void ImageWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPoint p(0, 0);
    painter.drawImage(p, _img);
}
/////////////////////////////////////////////////////////////////////////////////////
void ImageWindow::checkImgSize()
{
    if(_img.size() != _imgSize)
    {
        QRect rect;
        rect.setSize(_img.size());
        this->setGeometry(rect);
        this->move(QPoint(_xPos, _yPos));
        _imgSize = _img.size();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void ImageWindow::drawAll(ArtifactVector &picStars,
                          ArtifactVector &catStars,
                          ArtifactVector &eqPicStars,
                          ArtifactVector &eqCatStars)
{
    this->checkImgSize();
    draw::artifactMarks(_img, picStars);
    draw::starMarks(_img, catStars);
    draw::starConfig(_img, eqPicStars, 2, Qt::yellow);
    draw::starConfig(_img, eqCatStars, 1, Qt::cyan);
    update();
}
/////////////////////////////////////////////////////////////////////////////////////
void ImageWindow::inputFrame(Frame *f)
{
    f->lock().lockForRead();
    f->copyToQImage(_img);
    f->lock().unlock();

    QTime time;
    time.start();
    this->drawAll(_artifactBox.data(),
                  _starBox.data(),
                  _eqPicStars,
                  _eqCatStars);
    qDebug() << "drawing delay: " << time.elapsed();
}
/////////////////////////////////////////////////////////////////////////////////////
void ImageWindow::inputScreenStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _artifactBox = *a;
    a->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void ImageWindow::inputCatStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _starBox = *a;
    a->lock().unlock();

    draw::convertStarMagn(_starBox.data());
}
/////////////////////////////////////////////////////////////////////////////////////
void ImageWindow::inputEquatedStars(ArtifactBox *pic,
                                    ArtifactBox *cat)
{
    pic->lock().lockForRead();
    cat->lock().lockForRead();
    _eqPicStars = pic->data();
    _eqCatStars = cat->data();
    cat->lock().unlock();
    pic->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
