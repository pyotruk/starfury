#include "main.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QSettings settings("NIIPP", "astrobot");

    Strob strob(FRAME_WIDTH / 2, FRAME_HEIGHT / 2, &settings);
    RapidThread rapidThread(&settings);

    //form init
    w.initFace(strob.size(), (int)(strob.threshold()));

    //object <--> object connections
    QObject::connect(&rapidThread, SIGNAL(frame4RapidThread(void*,int,int)),
                     &strob, SLOT(makeTracking(void*,int,int)));
    QObject::connect(&rapidThread, SIGNAL(frame4SlowThread(void*,int,int)),
                     &w, SLOT(drawFrame(void*,int,int)));

    //gui --> object connections
    QObject::connect(&w, SIGNAL(unlockSlowBuf()),
                     rapidThread.getDoubleBuf(), SLOT(unlockSlowBuf()));
    QObject::connect(&w, SIGNAL(mousePressEvent(QMouseEvent *)),
                     &strob, SLOT(clickTarget(QMouseEvent *)));
    QObject::connect(&w, SIGNAL(changeStrobSize(int)),
                     &strob, SLOT(setSize(int)));
    QObject::connect(&w, SIGNAL(changeTrackingThreshold(int)),
                     &strob, SLOT(setThreshold(int)));

    w.show();
    return a.exec();
}
