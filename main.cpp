#include <QtGui/QApplication>
#include <QString>
#include <QObject>
#include <QSettings>
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////
#include "gui/controlwindow.h"
#include "gui/strobwnd.h"
#include "com/framerec.h"
#include "boxes/frame.h"
#include "detector/detector.h"
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
    Frame          frame0, frame1, frame2;
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
    Detector      detector(&settings,
                           &frame2,
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
    ControlWindow  controlWnd;
    StrobWindow    strobWnd;
    ImageWindow    detectorWnd;
    //gui init
    controlWnd.initFace(frameReceiver.strob().geometry().innerSide(),
                        (int)(frameReceiver.strob().threshold()),
                        angmeter.method(),
                        detector.accum().capacity());

    qDebug() << "QApplication a thread: " << a.thread();
    qDebug() << "frameReceiver thread: " << frameReceiver.thread();
    qDebug() << "detector thread:" << detector.thread();
    qDebug() << "starcatScreen thread: " << starcatScreen.thread();
    qDebug() << "angmeter thread: " << angmeter.thread();

    //object <--> object connections
    QObject::connect(&frameReceiver, SIGNAL(frame1Ready(Frame*,int,int)),
                     &detector, SLOT(inputFrame(Frame*,int,int)),
                     Qt::QueuedConnection);
    QObject::connect(&detector, SIGNAL(screenStarsReady(ArtifactBox*,double,double)),
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
                     &strobWnd, SLOT(inputFrame(Frame*)),
                     Qt::QueuedConnection);
    QObject::connect(&detector, SIGNAL(screenStarsReady(ArtifactBox*,double,double)),
                     &strobWnd, SLOT(inputScreenStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(catStarsReady(ArtifactBox*)),
                     &strobWnd, SLOT(inputCatStars(ArtifactBox*)),
                     Qt::QueuedConnection);    
    QObject::connect(&angmeter, SIGNAL(sendEquatedStars(ArtifactBox*,ArtifactBox*)),
                     &strobWnd, SLOT(inputEquatedStars(ArtifactBox*,ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&detector, SIGNAL(sendFrame(Frame*)),
                     &detectorWnd, SLOT(inputFrame(Frame*)),
                     Qt::QueuedConnection);

    QObject::connect(&strobWnd, SIGNAL(mousePressEvent(QMouseEvent *)),
                     &(frameReceiver.strob()), SLOT(clickTarget(QMouseEvent *)),
                     Qt::QueuedConnection);

    QObject::connect(&starcatScreen, SIGNAL(sendMeasureError(double,double)),
                     &controlWnd, SLOT(inputMeasureError(double,double)),
                     Qt::QueuedConnection);

    QObject::connect(&controlWnd, SIGNAL(changeStrobSize(int)),
                     &(frameReceiver.strob().geometry()), SLOT(setSide(int)),
                     Qt::QueuedConnection);
    QObject::connect(&controlWnd, SIGNAL(changeTrackingThreshold(int)),
                     &(frameReceiver.strob()), SLOT(setThreshold(int)),
                     Qt::QueuedConnection);
    QObject::connect(&controlWnd, SIGNAL(setSimtriMethod()),
                     &angmeter, SLOT(setSimtriMethod()),
                     Qt::QueuedConnection);
    QObject::connect(&controlWnd, SIGNAL(setFreevecMethod()),
                     &angmeter, SLOT(setFreevecMethod()),
                     Qt::QueuedConnection);

    QObject::connect(&controlWnd, SIGNAL(setAccumCapacity(int)),
                     &detector, SLOT(setAccumCapacity(int)),
                     Qt::QueuedConnection);
    QObject::connect(&controlWnd, SIGNAL(setBinEnabled(bool)),
                     &detector, SLOT(setBinEnabled(bool)),
                     Qt::QueuedConnection);

    controlWnd.show();
    strobWnd.show();
    detectorWnd.show();

    return a.exec();
}
