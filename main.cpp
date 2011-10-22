#include <QtGui/QApplication>
#include <QString>
#include <QObject>
#include "mainwindow.h"
#include "strob.h"
#include "readsharedmem.h"

#define FRAME_WIDTH  640
#define FRAME_HEIGHT 480
#define FRAME_HEADER_SIZE 32


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QString fileId  = "{TVA2-MemoryID}";
    QString eventId = "{TVA2-EventID}";
    QString mutexId = "{TVA2-MutexID}";

    SharedMem sharedMem((TCHAR*)fileId.data(),
                        (TCHAR*)eventId.data(),
                        (TCHAR*)mutexId.data(),
                        FRAME_HEADER_SIZE,
                        FRAME_WIDTH,
                        FRAME_HEIGHT);
    Strob strob(FRAME_WIDTH / 2, FRAME_HEIGHT / 2);

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
