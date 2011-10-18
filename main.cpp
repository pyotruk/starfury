#include <QtGui/QApplication>
#include <QString>
#include "main.h"
#include "mainwindow.h"
#include "readsharedmem.h"


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
    QObject::connect(&sharedMem, SIGNAL(DrawFrameSignal(void*, int, int)),
                     &w, SLOT(DrawFrameSlot(void*, int, int)));
    w.show();
    return a.exec();
}
