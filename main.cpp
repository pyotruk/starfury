#include "main.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QSettings settings("NIIPP", "astrobot");

    Strob strob(&settings);
    RapidThread rapidThread(&settings);

    //form init
    w.initFace(strob.geometry().innerSide(), (int)(strob.threshold()));

    //object <--> object connections
    QObject::connect(&rapidThread, SIGNAL(frame4Strob(Frame*)),
                     &strob, SLOT(makeTracking(Frame*)),
                     Qt::DirectConnection);
    QObject::connect(&rapidThread, SIGNAL(frame4Gui(Frame*)),
                     &w, SLOT(drawFrame(Frame*)),
                     Qt::QueuedConnection);

    //gui --> object connections
    QObject::connect(&w, SIGNAL(unlockSlowBuf()),
                     rapidThread.doubleBuf(), SLOT(unlockSlowBuf()),
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

    w.show();
    return a.exec();
}
