#include <QtGui/QApplication>
#include <QString>
#include <QObject>
#include <QSettings>
#include <QDebug>

#include "mainwindow.h"
#include "rapidthread.h"
#include "framerec.h"
#include "frame.h"
#include "stardetector.h"
#include "starcatscreen.h"
#include "snudpsrv.h"
#include "equator.h"

#define FRAME_HEADER_SIZE 32


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QSettings settings("NIIPP", "astrobot");

    RapidThread   rapidThread(&settings);
    FrameReceiver frameReceiver(&settings);
    StarDetector  starDetector(&settings);
    StarcatScreen starcatScreen(&settings);
    Equator       equator(&settings);

    qDebug() << "QApplication a thread: " << a.thread();
    qDebug() << "MainWidow w thread: " << w.thread();
    qDebug() << "rapidThread thread: " << rapidThread.thread();
    qDebug() << "frameReceiver thread: " << frameReceiver.thread();
    qDebug() << "starDetector thread:" << starDetector.thread();
    qDebug() << "starcatScreen thread: " << starcatScreen.thread();
    qDebug() << "equator thread: " << equator.thread();

    //form init
    w.initFace(rapidThread.strob()->geometry().innerSide(),
               (int)(rapidThread.strob()->threshold()));

    //object <--> object connections
    QObject::connect(&frameReceiver, SIGNAL(frameReady(Frame*, QMutex*)),
                     &rapidThread, SLOT(frameIn(Frame*, QMutex*)),
                     Qt::QueuedConnection);
    QObject::connect(&rapidThread, SIGNAL(frameOut0(Frame*,QMutex*,int,int)),
                     &starDetector, SLOT(frameIn(Frame*,QMutex*,int,int)),
                     Qt::QueuedConnection);
    QObject::connect(&rapidThread, SIGNAL(frameOut1(Frame*, QMutex*)),
                     &w, SLOT(drawFrame(Frame*, QMutex*)),
                     Qt::QueuedConnection);
    QObject::connect(&starDetector, SIGNAL(artifactsOut(ArtifactVector*,QMutex*,QDateTime*)),
                     &equator, SLOT(inputArtifacts(ArtifactVector*,QMutex*,QDateTime*)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(starsReady(ArtifactVector*,QMutex*,QDateTime*)),
                     &equator, SLOT(inputStars(ArtifactVector*,QMutex*,QDateTime*)),
                     Qt::QueuedConnection);

    //gui <--> object connections
    QObject::connect(&equator, SIGNAL(toGui(ArtifactVector*,ArtifactVector*,QMutex*)),
                     &w, SLOT(fromEquator(ArtifactVector*,ArtifactVector*,QMutex*)),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(mousePressEvent(QMouseEvent *)),
                     rapidThread.strob(), SLOT(clickTarget(QMouseEvent *)),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(changeStrobSize(int)),
                     &(rapidThread.strob()->geometry()), SLOT(setSide(int)),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(changeTrackingThreshold(int)),
                     rapidThread.strob(), SLOT(setThreshold(int)),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(screenSizeChanged(int,int)),
                     &starcatScreen, SLOT(setScreenSize(int,int)),
                     Qt::QueuedConnection);

    w.show();
    return a.exec();
}
