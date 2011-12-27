#include <QtGui/QApplication>
#include <QString>
#include <QObject>
#include <QSettings>
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////
#include "gui/controlwindow.h"
#include "gui/imagewindow.h"
#include "com/framerec.h"
#include "boxes/frame.h"
#include "detector/stardetector.h"
#include "sky/starcatscreen.h"
#include "com/snudpsrv.h"
#include "astrometry/angmeter.h"
#include "common/logfile.h"
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //through obj
    QSettings settings("petr_klukvenny", "starfury");
    LogFile targetLog("starfury-target");
    LogFile starsLog("starfury-stars");
    LogFile strobLog("starfury-strob");

    //per-thread data
    Frame          frame0, frame1;
    ArtifactBox    rawPicStars;
    ArtifactBox    rawCatStars;
    ArtifactBox    equatedPicStars;
    ArtifactBox    equatedCatStars;
    TargetBox      target;

    //singletons
    Strob         strob(&settings,
                        &strobLog);
    FrameReceiver frameReceiver(&settings,
                                &strob,
                                &frame0,
                                &frame1);
    StarDetector  starDetector(&settings,
                               &rawPicStars);
    StarcatScreen starcatScreen(&settings,
                                &targetLog,
                                &rawCatStars);
    Angmeter      angmeter(&settings,
                           &starsLog,
                           &equatedPicStars,
                           &equatedCatStars,
                           &target);

    //gui
    ControlWindow controlWin;
    ImageWindow   imgWin;
    //gui init
    controlWin.initFace(frameReceiver.strob().geometry().innerSide(),
                        (int)(frameReceiver.strob().threshold()),
                        angmeter.method());

    qDebug() << "QApplication a thread: " << a.thread();
    qDebug() << "frameReceiver thread: " << frameReceiver.thread();
    qDebug() << "starDetector thread:" << starDetector.thread();
    qDebug() << "starcatScreen thread: " << starcatScreen.thread();
    qDebug() << "angmeter thread: " << angmeter.thread();

    //object <--> object connections
    QObject::connect(&frameReceiver, SIGNAL(frame1Ready(Frame*,int,int)),
                     &starDetector, SLOT(inputFrame(Frame*,int,int)),
                     Qt::QueuedConnection);
    QObject::connect(&starDetector, SIGNAL(screenStarsReady(ArtifactBox*,double,double)),
                     &angmeter, SLOT(inputScreenStars(ArtifactBox*,double,double)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(catStarsReady(ArtifactBox*)),
                     &angmeter, SLOT(inputCatStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&frameReceiver, SIGNAL(frameSizeChanged(int,int)),
                     &starcatScreen, SLOT(setScreenSize(int,int)));
    QObject::connect(&frameReceiver, SIGNAL(frameSizeChanged(int,int)),
                     &angmeter, SLOT(setScreenSize(int,int)),
                     Qt::QueuedConnection);
    QObject::connect(&angmeter, SIGNAL(sendTarget(TargetBox*)),
                     &starcatScreen, SLOT(inputTarget(TargetBox*)),
                     Qt::QueuedConnection);

    //gui <--> object connections
    QObject::connect(&frameReceiver, SIGNAL(frame0Ready(Frame*)),
                     &imgWin, SLOT(inputFrame(Frame*)),
                     Qt::QueuedConnection);
    QObject::connect(&starDetector, SIGNAL(screenStarsReady(ArtifactBox*,double,double)),
                     &imgWin, SLOT(inputScreenStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(catStarsReady(ArtifactBox*)),
                     &imgWin, SLOT(inputCatStars(ArtifactBox*)),
                     Qt::QueuedConnection);    
    QObject::connect(&angmeter, SIGNAL(sendEquatedStars(ArtifactBox*,ArtifactBox*)),
                     &imgWin, SLOT(inputEquatedStars(ArtifactBox*,ArtifactBox*)),
                     Qt::QueuedConnection);

    QObject::connect(&imgWin, SIGNAL(mousePressEvent(QMouseEvent *)),
                     &(frameReceiver.strob()), SLOT(clickTarget(QMouseEvent *)),
                     Qt::QueuedConnection);

    QObject::connect(&starcatScreen, SIGNAL(sendMeasureError(double,double)),
                     &controlWin, SLOT(inputMeasureError(double,double)),
                     Qt::QueuedConnection);

    QObject::connect(&controlWin, SIGNAL(changeStrobSize(int)),
                     &(frameReceiver.strob().geometry()), SLOT(setSide(int)),
                     Qt::QueuedConnection);
    QObject::connect(&controlWin, SIGNAL(changeTrackingThreshold(int)),
                     &(frameReceiver.strob()), SLOT(setThreshold(int)),
                     Qt::QueuedConnection);
    QObject::connect(&controlWin, SIGNAL(setSimtriMethod()),
                     &angmeter, SLOT(setSimtriMethod()));
    QObject::connect(&controlWin, SIGNAL(setFreevecMethod()),
                     &angmeter, SLOT(setFreevecMethod()));

    controlWin.show();
    imgWin.show();
    return a.exec();
}
