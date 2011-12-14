#include <QtGui/QApplication>
#include <QString>
#include <QObject>
#include <QSettings>
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////
#include "mainwindow.h"
#include "framerec.h"
#include "frame.h"
#include "stardetector.h"
#include "starcatscreen.h"
#include "snudpsrv.h"
#include "angmeter.h"
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
    Frame          frame0, frame1;
    ArtifactBox    screenStars;
    ArtifactBox    catStars;

    //threads
    FrameReceiver frameReceiver(&settings,
                                &frame0,
                                &frame1);
    StarDetector  starDetector(&settings,
                               &screenStars);
    StarcatScreen starcatScreen(&settings,
                                &catStars);
    Angmeter      angmeter(&settings);

    qDebug() << "QApplication a thread: " << a.thread();
    qDebug() << "MainWidow w thread: " << w.thread();
    qDebug() << "frameReceiver thread: " << frameReceiver.thread();
    qDebug() << "starDetector thread:" << starDetector.thread();
    qDebug() << "starcatScreen thread: " << starcatScreen.thread();
    qDebug() << "angmeter thread: " << angmeter.thread();

    //form init
    w.initFace(frameReceiver.strob().geometry().innerSide(),
               (int)(frameReceiver.strob().threshold()));

    //object <--> object connections
    QObject::connect(&frameReceiver, SIGNAL(frame1Ready(Frame*,int,int)),
                     &starDetector, SLOT(inputFrame(Frame*,int,int)),
                     Qt::QueuedConnection);
    QObject::connect(&starDetector, SIGNAL(screenStarsReady(ArtifactBox*,int,int)),
                     &angmeter, SLOT(inputScreenStars(ArtifactBox*,int,int)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(catStarsReady(ArtifactBox*)),
                     &angmeter, SLOT(inputCatStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&frameReceiver, SIGNAL(frameSizeChanged(int,int)),
                     &starcatScreen, SLOT(setScreenSize(int,int)));
    QObject::connect(&frameReceiver, SIGNAL(frameSizeChanged(int,int)),
                     &angmeter, SLOT(setScreenSize(int,int)),
                     Qt::QueuedConnection);
    QObject::connect(&angmeter, SIGNAL(sendTarget(int,int)),
                     &starcatScreen, SLOT(inputTarget(int,int)),
                     Qt::QueuedConnection);

    //gui <--> object connections
    QObject::connect(&frameReceiver, SIGNAL(frame0Ready(Frame*)),
                     &w, SLOT(inputFrame(Frame*)),
                     Qt::QueuedConnection);

    QObject::connect(&starDetector, SIGNAL(screenStarsReady(ArtifactBox*,int,int)),
                     &w, SLOT(inputScreenStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(catStarsReady(ArtifactBox*)),
                     &w, SLOT(inputCatStars(ArtifactBox*)),
                     Qt::QueuedConnection);

    QObject::connect(&angmeter, SIGNAL(sendTriangles(TriangleBox*)),
                     &w, SLOT(inputTriangles(TriangleBox*)),
                     Qt::QueuedConnection);

    QObject::connect(&w, SIGNAL(mousePressEvent(QMouseEvent *)),
                     &(frameReceiver.strob()), SLOT(clickTarget(QMouseEvent *)),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(changeStrobSize(int)),
                     &(frameReceiver.strob().geometry()), SLOT(setSide(int)),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(changeTrackingThreshold(int)),
                     &(frameReceiver.strob()), SLOT(setThreshold(int)),
                     Qt::QueuedConnection);

    w.show();
    return a.exec();
}
