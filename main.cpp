#include <QtGui/QApplication>
#include <QString>
#include <QObject>
#include <QSettings>
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////
#include "gui/control_wnd.h"
#include "gui/strob_wnd.h"
#include "gui/detector_wnd.h"
#include "gui/photometry_wnd.h"
#include "com/framerec.h"
#include "boxes/frame.h"
#include "detector/detector.h"
#include "sky/starcatscreen.h"
#include "astrometry/angmeter.h"
#include "common/logfile.h"
#include "strob/strob_wrapper.h"
#include "boxes/telescope.h"
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* common objects */
    QSettings settings("petr_klukvenny", "starfury");
    LogFile targetLog("starfury-target");
    LogFile starsLog("starfury-stars");
    LogFile strobLog("starfury-strob");

    /* per-thread data */
    FrameBox       frame0, frame1, frame2;
    ArtifactBox    shared_PicStars;
    ArtifactBox    shared_CatStars;
    ArtifactBox    shared_EqPicStars; //equated pic stars
    ArtifactBox    shared_EqCatStars; //equated cat stars
    ArtifactBox    shared_Targets;

    /* singletons */
    StrobWrapper  strobWrapper(&settings,
                               &strobLog,
                               &shared_Targets);
    FrameReceiver frameReceiver(&settings,
                                &frame0,
                                &frame1);
    Detector      detector(&settings,
                           &frame2,
                           &shared_PicStars,
                           &shared_Targets);
    StarcatScreen starcatScreen(&settings,
                                &shared_CatStars);
    Angmeter      angmeter(&settings,
                           &starsLog,
                           &targetLog,
                           &shared_EqPicStars,
                           &shared_EqCatStars);
    Telescope     telescope(&settings);

    /* gui */
    ControlWindow    controlWnd;
    StrobWindow      strobWnd("tracking", &settings);
    DetectorWindow   detectorWnd("detection", &settings);
    PhotometryWindow photometryWnd("photometry", &settings);
    /* gui init */
    controlWnd.initFace(strobWrapper.strob().geometry().side(),
                        angmeter.method(),
                        detector.accum().capacity(),
                        detector.mode());
    detectorWnd.setMode(detector.mode());

    qDebug() << "QApplication :" << a.thread();
    qDebug() << strobWrapper.thread();
    qDebug() << frameReceiver.thread();
    qDebug() << detector.thread();
    qDebug() << starcatScreen.thread();
    qDebug() << angmeter.thread();
    qDebug() << "Telescope :" <<telescope.thread();

    /* object --> object connections */
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
    QObject::connect(&starcatScreen, SIGNAL(sendScreenVelocity(double,double)),
                     &detector, SLOT(inputScreenVelocity(double,double)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(sendScreenVelocity(double,double)),
                     &strobWrapper, SLOT(setVelocity(double,double)),
                     Qt::QueuedConnection);
    QObject::connect(&telescope, SIGNAL(sendTelPos(TelBox*)),
                     &starcatScreen, SLOT(inputTelPos(TelBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&telescope, SIGNAL(sendTelPos(TelBox*)),
                     &angmeter, SLOT(inputTelPos(TelBox*)),
                     Qt::QueuedConnection);


    /* object --> gui connections */
    QObject::connect(&strobWrapper, SIGNAL(frameReady(FrameBox*)),
                     &strobWnd, SLOT(inputFrame(FrameBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&detector, SIGNAL(starsReady(ArtifactBox*)),
                     &detectorWnd, SLOT(inputPicStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&starcatScreen, SIGNAL(catStarsReady(ArtifactBox*)),
                     &detectorWnd, SLOT(inputCatStars(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&angmeter, SIGNAL(sendEquatedStars(ArtifactBox*,ArtifactBox*)),
                     &detectorWnd, SLOT(inputEquatedStars(ArtifactBox*,ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&detector, SIGNAL(sendFrame(FrameBox*)),
                     &detectorWnd, SLOT(inputFrame(FrameBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&detector, SIGNAL(targetsReady(ArtifactBox*)),
                     &detectorWnd, SLOT(inputTargets(ArtifactBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&strobWrapper, SIGNAL(sendPhotometry(double,double,double)),
                     &photometryWnd, SLOT(addPoint(double,double,double)),
                     Qt::QueuedConnection);
    QObject::connect(&angmeter, SIGNAL(sendMeasureError(double,double)),
                     &controlWnd, SLOT(inputMeasureError(double,double)),
                     Qt::QueuedConnection);


    /* gui --> object connections */
    QObject::connect(&strobWnd, SIGNAL(mousePressEvent(QMouseEvent *)),
                     &strobWrapper, SLOT(setPos(QMouseEvent*)),
                     Qt::QueuedConnection);
    QObject::connect(&controlWnd, SIGNAL(setStrobSize(int)),
                     &strobWrapper, SLOT(setSide(int)),
                     Qt::QueuedConnection);
    QObject::connect(&controlWnd, SIGNAL(setAstrometryMethod(int)),
                     &angmeter, SLOT(setMethod(int)),
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


    /* gui --> gui connections */
    QObject::connect(&controlWnd, SIGNAL(setDetectorMode(int)),
                     &detectorWnd, SLOT(setMode(int)));

    /* application shutdown */
    QObject::connect(&controlWnd, SIGNAL(closed()),
                     &a, SLOT(closeAllWindows()));

    controlWnd.show();
    strobWnd.show();
    detectorWnd.show();
    photometryWnd.show();

    return a.exec();
}
