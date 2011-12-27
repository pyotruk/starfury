#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QMainWindow>
#include <QObject>
#include <QMouseEvent>
#include <QImage>
#include <QPainter>
#include <QPoint>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/artifact.h"
#include "boxes/frame.h"
#include "drawing.h"
/////////////////////////////////////////////////////////////////////////////////////
class ImageWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ImageWindow(QWidget *parent = 0);
    ~ImageWindow();
private:
    static const int _xPos = 200;
    static const int _yPos = 200;
    static const int _timeout = 20;
    static const int _crossSide = 10;
    QImage          _img;
    QSize           _imgSize;
    ArtifactBox     _artifactBox;
    ArtifactBox     _starBox;
    ArtifactVector  _eqPicStars;
    ArtifactVector  _eqCatStars;
    void checkImgSize();
    void drawAll(ArtifactVector &picStars,
                 ArtifactVector &catStars,
                 ArtifactVector &eqPicStars,
                 ArtifactVector &eqCatStars);
private slots:
    void paintEvent(QPaintEvent *);
    void inputFrame(Frame*);
    void inputScreenStars(ArtifactBox*);
    void inputCatStars(ArtifactBox*);
    void inputEquatedStars(ArtifactBox *pic,
                           ArtifactBox *cat);
signals:
    void mousePressEvent(QMouseEvent *);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // IMAGEWINDOW_H
