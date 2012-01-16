#include <QtGui/QApplication>
#include <QString>
#include <QObject>
#include <QSettings>
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////
#include "gui/controlwindow.h"
#include "gui/strobwnd.h"
#include "gui/detector_wnd.h"
#include "gui/photometry_wnd.h"
#include "com/framerec.h"
#include "boxes/frame.h"
#include "detector/detector.h"
#include "sky/starcatscreen.h"
#include "com/snudpsrv.h"
#include "astrometry/angmeter.h"
#include "common/logfile.h"
#include "strob/strob_wrapper.h"
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
    FrameBox       frame0, frame1, frame2;
    ArtifactBox    rawPicStars;
    ArtifactBox    rawCatStars;
    ArtifactBox    equatedPicStars;
    ArtifactBox    equatedCatStars;
    ArtifactBox    targets;

    //singletons
    StrobWrapper  strobWrapper(&settings,
                               &strobLog,
                               &targets);
    FrameReceiver frameReceiver(&settings,
                                &frame0,
                                &frame1);
    Detector      detector(&settings,
                           &frame2,
                           &rawPicStars,
                           &targets);
    StarcatScreen starcatScreen(&settings,
                                &targetLog,
                                &rawCatStars);
    Angmeter      angmeter(&settings,
                           &starsLog,
                           &equatedPicStars,
                           &equatedCatStars,
                           &targets);

    //gui
    ControlWindow    controlWnd;
    StrobWindow      strobWnd("tracking", QPoint(100, 100));
    DetectorWindow   detectorWnd("detection", QPoint(200, 200));
    PhotometryWindow photometryWnd(&settings);
    //gui init
    controlWnd.initFace(strobWrapper.strob().geometry().side(),
                        (int)(strobWrapper.strob().threshold()),
                        angmeter.method(),
                        detector.accum().capacity(),
                        detector.mode());

    qDebug() << "QApplication " << a.thread();
    qDebug() << frameReceiver.thread();
    qDebug() << detector.thread();
    qDebug() << starcatScreen.thread();
    qDebug() << angmeter.thread();

    //object <--> object connections
    QObject::connect(&frameReceiver, SIGNAL(frame0Ready(FrameBox*)),
                     &strobWrapper, SLOT(inputFrame(FrameBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&frameReceiver, SIGNAL(frame1Ready(FrameBox*)),
                     &detector, SLOT(inputFrame(FrameBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&strobWrapper, SIGNAL(freshTargets(ArtifactBox*)),
                     &angmeter, SLOT(inputTargets(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&detector, SIGNAL(targetsReady(ArtifactBox*)),
                     &strobWrapper, SLOT(targetsDetected()),
                     Qt::QueuedConnection);
    QObject::connect(&detector, SIGNAL(starsReady(ArtifactBox*)),
                     &angmeter, SLOT(inputPicStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(catStarsReady(ArtifactBox*)),
                     &angmeter, SLOT(inputCatStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&frameReceiver, SIGNAL(frameSizeChanged(int,int)),
                     &starcatScreen, SLOT(setScreenSize(int,int)));
    QObject::connect(&frameReceiver, SIGNAL(frameSizeChanged(int,int)),
                     &angmeter, SLOT(setScreenSize(int,int)),
                     Qt::QueuedConnection);
    QObject::connect(&angmeter, SIGNAL(sendTargets(ArtifactBox*)),
                     &starcatScreen, SLOT(inputTarget(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(sendScreenVelocity(double,double)),
                     &detector, SLOT(inputScreenVelocity(double,double)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(sendScreenVelocity(double,double)),
                     &strobWrapper, SLOT(setVelocity(double,double)),
                     Qt::QueuedConnection);

    //gui <--> object connections
    QObject::connect(&strobWrapper, SIGNAL(frameReady(FrameBox*)),
                     &strobWnd, SLOT(inputFrame(FrameBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&detector, SIGNAL(starsReady(ArtifactBox*)),
                     &strobWnd, SLOT(inputPicStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(catStarsReady(ArtifactBox*)),
                     &strobWnd, SLOT(inputCatStars(ArtifactBox*)),
                     Qt::QueuedConnection);    
    QObject::connect(&angmeter, SIGNAL(sendEquatedStars(ArtifactBox*,ArtifactBox*)),
                     &strobWnd, SLOT(inputEquatedStars(ArtifactBox*,ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&detector, SIGNAL(sendFrame(FrameBox*)),
                     &detectorWnd, SLOT(inputFrame(FrameBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&detector, SIGNAL(targetsReady(ArtifactBox*)),
                     &detectorWnd, SLOT(inputTargets(ArtifactBox*)),
                     Qt::QueuedConnection);

    QObject::connect(&strobWnd, SIGNAL(mousePressEvent(QMouseEvent *)),
                     &strobWrapper, SLOT(setPos(QMouseEvent*)),
                     Qt::QueuedConnection);

    QObject::connect(&strobWrapper, SIGNAL(sendPhotometry(double,double,double)),
                     &photometryWnd, SLOT(addPoint(double,double,double)),
                     Qt::QueuedConnection);

    QObject::connect(&starcatScreen, SIGNAL(sendMeasureError(double,double)),
                     &controlWnd, SLOT(inputMeasureError(double,double)),
                     Qt::QueuedConnection);

    QObject::connect(&controlWnd, SIGNAL(changeStrobSize(int)),
                     &strobWrapper, SLOT(setSide(int)),
                     Qt::QueuedConnection);
    QObject::connect(&controlWnd, SIGNAL(changeTrackingThreshold(int)),
                     &strobWrapper, SLOT(setThreshold(int)),
                     Qt::QueuedConnection);
    QObject::connect(&controlWnd, SIGNAL(setSimtriMethod()),
                     &angmeter, SLOT(setSimtriMethod()),
                     Qt::QueuedConnection);
    QObject::connect(&controlWnd, SIGNAL(setFreevecMethod()),
                     &angmeter, SLOT(setFreevecMethod()),
                     Qt::QueuedConnection);

    QObject::connect(&controlWnd, SIGNAL(setDetectorMode(int)),
                     &detector, SLOT(setMode(int)),
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
    photometryWnd.show();

    return a.exec();
}
