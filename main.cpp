#include <QtGui/QApplication>
#include <QString>
#include "mainwindow.h"
#include "frameproc.h"
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
    FrameProc frameProc;
    QObject::connect(&sharedMem, SIGNAL(FrameProcSignal(void*, int, int)),
                     &frameProc, SLOT(FrameProcSlot(void*, int, int)));
    QObject::connect(&frameProc, SIGNAL(DrawFrameSignal(void*, int, int)),
                     &w, SLOT(DrawFrameSlot(void*, int, int)));
    w.show();
    return a.exec();
}
