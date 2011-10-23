#include "main.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QSettings settings("NIIPP", "astrobot");

    SharedMem sharedMem(FRAME_HEADER_SIZE,
                        FRAME_WIDTH,
                        FRAME_HEIGHT,
                        &settings);
    Strob strob(FRAME_WIDTH / 2, FRAME_HEIGHT / 2, &settings);

    //form init
    w.initFace(strob.size());

    //connections
    QObject::connect(&sharedMem, SIGNAL(frameRecived(void*,int,int)),
                     &strob, SLOT(makeTracking(void*,int,int)));
    QObject::connect(&strob, SIGNAL(drawFrame(void*, int, int)),
                     &w, SLOT(drawFrame(void*, int, int)));
    QObject::connect(&w, SIGNAL(mousePressEvent(QMouseEvent *)),
                     &strob, SLOT(clickTarget(QMouseEvent *)));
    QObject::connect(&w, SIGNAL(changeStrobSize(int)),
                     &strob, SLOT(setSize(int)));

    w.show();
    return a.exec();
}
