#ifndef READSHAREDMEM_H
#define READSHAREDMEM_H

#include <QDebug>
#include <QThread>
#include "windows.h"
#include "qt_windows.h"

typedef unsigned char uchar;

struct FrameSizes
{
    int headerSize;
    int width;
    int height;
};

class SharedMem : public QThread
{
    Q_OBJECT
public:
    SharedMem(LPCTSTR fileMapId,
              LPCTSTR eventId,
              LPCTSTR mutexId,
              int frameHeaderSize,
              int frameWidth,
              int frameHeight);
    ~SharedMem();
protected:
    void run();
private:
    void       *FpSharedBuf;
    HANDLE     FhMappedFile;
    HANDLE     FhEvent;
    HANDLE     FhMutex;
    uchar      *FpFrame;
    FrameSizes FFrameSizes;
    void ReadBuf();
signals:
    void StrobSignal(void   *pFrame,
                     int frameWidth,
                     int frameHeight); //сигнал на оработку картинки
};

#endif // READSHAREDMEM_H
