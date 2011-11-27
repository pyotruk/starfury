#include <QtGui/QApplication>
#include <QString>
#include <QObject>
#include <QSettings>
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////
#include "mainwindow.h"
#include "framerec.h"
#include "frame.h"
#include "strob.h"
#include "stardetector.h"
#include "starcatscreen.h"
#include "snudpsrv.h"
#include "equator.h"
#include "artifactbox.h"
/////////////////////////////////////////////////////////////////////////////////////
#define FRAME_HEADER_SIZE 32
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QSettings settings("NIIPP", "astrobot");

    //per-thread data
    Frame          frame;
    ArtifactBox    screenStars;
    ArtifactBox    catStars;

    //threads
    FrameReceiver frameReceiver(&settings,
                                &frame);
    Strob         strob(&settings);
    StarDetector  starDetector(&settings,
                               &screenStars);
    StarcatScreen starcatScreen(&settings,
                                &catStars);
    Equator       equator(&settings);

    qDebug() << "QApplication a thread: " << a.thread();
    qDebug() << "MainWidow w thread: " << w.thread();
    qDebug() << "frameReceiver thread: " << frameReceiver.thread();
    qDebug() << "strob thread: " << strob.thread();
    qDebug() << "starDetector thread:" << starDetector.thread();
    qDebug() << "starcatScreen thread: " << starcatScreen.thread();
    qDebug() << "equator thread: " << equator.thread();

    //form init
    w.initFace(strob.geometry().innerSide(),
               (int)(strob.threshold()));

    //object <--> object connections
    QObject::connect(&frameReceiver, SIGNAL(frameReady(Frame*)),
                     &strob, SLOT(makeTracking(Frame*)),
                     Qt::QueuedConnection);
    QObject::connect(&strob, SIGNAL(frameReady(Frame*,int,int)),
                     &starDetector, SLOT(inputFrame(Frame*,int,int)),
                     Qt::QueuedConnection);
    QObject::connect(&starDetector, SIGNAL(screenStarsReady(ArtifactBox*)),
                     &equator, SLOT(inputScreenStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(catStarsReady(ArtifactBox*)),
                     &equator, SLOT(inputCatStars(ArtifactBox*)),
                     Qt::QueuedConnection);

    //gui <--> object connections
    QObject::connect(&strob, SIGNAL(frameReady(Frame*,int,int)),
                     &w, SLOT(drawFrame(Frame*)),
                     Qt::QueuedConnection);
    QObject::connect(&starDetector, SIGNAL(screenStarsReady(ArtifactBox*)),
                     &w, SLOT(inputScreenStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(catStarsReady(ArtifactBox*)),
                     &w, SLOT(inputCatStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(mousePressEvent(QMouseEvent *)),
                     &strob, SLOT(clickTarget(QMouseEvent *)),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(changeStrobSize(int)),
                     &(strob.geometry()), SLOT(setSide(int)),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(changeTrackingThreshold(int)),
                     &strob, SLOT(setThreshold(int)),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(screenSizeChanged(int,int)),
                     &starcatScreen, SLOT(setScreenSize(int,int)),
                     Qt::QueuedConnection);

    w.show();
    return a.exec();
}
