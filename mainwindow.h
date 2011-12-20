#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QMainWindow>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QtAlgorithms>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/frame.h"
#include "utils/drawhelpfun.h"
#include "boxes/artifact.h"
/////////////////////////////////////////////////////////////////////////////////////

namespace Ui {
    class MainWindow;
}
/////////////////////////////////////////////////////////////////////////////////////
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initFace(const int strobSize,
                  const int trackTresh);
private:
    static const double __deg2rad = 0.017453292519943295769236907684886;
    static const double __rad2deg = 57.295779513082320876798154814105;
    static const int _timeout = 20;
    static const int _crossSide = 10;
    Ui::MainWindow *ui;
    QImage          _img;
    QSize           _imgSize;
    ArtifactBox     _artifactBox;
    ArtifactBox     _starBox;
    ArtifactVector  _eqPicStars;
    ArtifactVector  _eqCatStars;
    void checkImgSize();
    void adaptWindowSize(const QSize&);
    void markArtifacts(QImage&,
                       ArtifactVector&);
    void markStars(QImage&,
                   ArtifactVector&);
    void drawStarConfig(ArtifactVector&,
                        const int width,
                        const QColor&);
    void drawAll(ArtifactVector &picStars,
                 ArtifactVector &catStars,
                 ArtifactVector &eqPicStars,
                 ArtifactVector &eqCatStars);
    void convertStarMagn(ArtifactVector&); //пересчёт магнитуды звезды из каталога (зв.в.) в картинку
private slots:
    void inputFrame(Frame*);
    void inputScreenStars(ArtifactBox*);
    void inputCatStars(ArtifactBox*);
    void inputEquatedStars(ArtifactBox *pic,
                           ArtifactBox *cat);
    void inputMeasureError(double errAlpha,  //rad
                           double errDelta); //rad
    void paintEvent(QPaintEvent *);
    void updateFace();
signals:
    void mousePressEvent(QMouseEvent *);
    void changeStrobSize(const int);
    void changeTrackingThreshold(const int);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // MAINWINDOW_H
