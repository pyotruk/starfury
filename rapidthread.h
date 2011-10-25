#ifndef RAPIDTHREAD_H
#define RAPIDTHREAD_H

#include <QThread>
#include <QSettings>
#include <QSize>
#include "readsharedmem.h"
#include "doublebuffer.h"

class RapidThread : public QThread
{
    Q_OBJECT
public:
    explicit RapidThread(QSettings *settings = 0);
    ~RapidThread();
    DoubleBuffer *getDoubleBuf();
protected:
    void run();
private:
    static const int DEFAULT_FRAME_WIDTH  = 640;
    static const int DEFAULT_FRAME_HEIGHT = 480;
    QSize        FFrameSize;
    DoubleBuffer *FDoubleBuf;
    SharedMem    *FSharedMem;
    FrameHeader  FFrameHeader;
signals:
    void frame4RapidThread(void *frame,
                           int  frameWidth,
                           int  frameHeight); //сигнал для быстрого потока (оработка)
    void frame4SlowThread(void *frame,
                          int  frameWidth,
                          int  frameHeight); //сигнал для медленного потока (отображение)
};

#endif // RAPIDTHREAD_H
