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
    QObject::connect(&rapidThread, SIGNAL(frame4Strob(void*,const int,const int)),
                     &strob, SLOT(makeTracking(void*,const int,const int)),
                     Qt::DirectConnection);
    QObject::connect(&rapidThread, SIGNAL(frame4Gui(const void*,const int,const int)),
                     &w, SLOT(drawFrame(const void*,const int,const int)),
                     Qt::QueuedConnection);

    //gui --> object connections
    QObject::connect(&w, SIGNAL(unlockSlowBuf()),
                     rapidThread.getDoubleBuf(), SLOT(unlockSlowBuf()),
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
