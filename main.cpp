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

    //objects connections
    /* GUI connection in MainWidow::MainWindow() */
    QObject::connect(&rapidThread, SIGNAL(frameReceived(void*,int,int)),
                     &strob, SLOT(makeTracking(void*,int,int)));
    QObject::connect(&strob, SIGNAL(drawFrame(void*,int,int)),
                     &w, SLOT(drawFrame(void*,int,int)));
    QObject::connect(&w, SIGNAL(mousePressEvent(QMouseEvent *)),
                     &strob, SLOT(clickTarget(QMouseEvent *)));
    QObject::connect(&w, SIGNAL(changeStrobSize(int)),
                     &strob, SLOT(setSize(int)));
    QObject::connect(&w, SIGNAL(changeTrackingThreshold(int)),
                     &strob, SLOT(setThreshold(int)));

    w.show();
    return a.exec();
}
