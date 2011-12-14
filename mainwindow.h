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
#include "astrometry/triangle.h"
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
    static const int _timeout = 20;
    static const int _crossSide = 10;
    Ui::MainWindow *ui;
    QImage          _img;
    QSize           _imgSize;
    ArtifactBox     _artifactBox;
    ArtifactBox     _starBox;
    TriangleBox     _tribox;
    void checkImgSize();
    void adaptWindowSize(const QSize&);
    void markArtifacts(QImage&,
                       ArtifactVector&);
    void markStars(QImage&,
                   ArtifactVector&);
    void drawTriangles(TriangleVector&,
                       const int width,
                       const QColor&);
    void drawAll(TriangleBoxData&,
                 ArtifactVector &picStars,
                 ArtifactVector &catStars);
private slots:
    void inputFrame(Frame*);
    void inputScreenStars(ArtifactBox*);
    void inputCatStars(ArtifactBox*);
    void inputTriangles(TriangleBox*);
    void paintEvent(QPaintEvent *);
    void updateFace();
signals:
    void mousePressEvent(QMouseEvent *);
    void changeStrobSize(const int);
    void changeTrackingThreshold(const int);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // MAINWINDOW_H
