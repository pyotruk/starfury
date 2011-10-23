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
    w.InitFace(strob.Size());

    //connections
    QObject::connect(&sharedMem, SIGNAL(StrobSignal(void*, int, int)),
                     &strob, SLOT(StrobSlot(void*, int, int)));
    QObject::connect(&strob, SIGNAL(DrawFrameSignal(void*, int, int)),
                     &w, SLOT(DrawFrameSlot(void*, int, int)));
    QObject::connect(&w, SIGNAL(mousePressEvent(QMouseEvent *)),
                     &strob, SLOT(ClickTarget(QMouseEvent *)));
    QObject::connect(&w, SIGNAL(StrobSizeChangedSignal(int)),
                     &strob, SLOT(StrobChange(int)));

    w.show();
    return a.exec();
}
