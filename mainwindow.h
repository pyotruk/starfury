#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QMainWindow>
#include <QDebug>
#include <QTime>
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QMutex>
#include <QVector>
#include <QtAlgorithms>
#include "frame.h"
#include "artifact.h"
#include "drawhelpfun.h"
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
    ArtifactVector  _artVec;
    ArtifactVector  _stars;
    void adaptWindowSize(const QSize&);
    void markArtifacts(QImage&);
    void markStars(QImage&);
private slots:
    void drawFrame(Frame*, QMutex*);
    void inputArtifacts(ArtifactVector*, QMutex*);
    void inputStars(ArtifactVector*, QMutex*);
    void paintEvent(QPaintEvent *);
    void updateFace();
signals:
    void mousePressEvent(QMouseEvent *);
    void changeStrobSize(const int);
    void changeTrackingThreshold(const int);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // MAINWINDOW_H
