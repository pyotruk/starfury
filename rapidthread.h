#ifndef RAPIDTHREAD_H
#define RAPIDTHREAD_H

#include <QThread>
#include <QSettings>
#include "readsharedmem.h"

typedef unsigned char uchar;

class RapidThread : public QThread
{
    Q_OBJECT
public:
    explicit RapidThread(QSettings *settings = 0);
    ~RapidThread();
protected:
    void run();
private:
    static const int DEFAULT_FRAME_WIDTH  = 640;
    static const int DEFAULT_FRAME_HEIGHT = 480;
    SharedMem   *FSharedMem;
    uchar       *FFrame;
    FrameHeader FFrameHeader;
    void checkFrameSize(const FrameHeader &header,
                        uchar *frame);
signals:
    void frameReceived(void *pFrame,
                       int  frameWidth,
                       int  frameHeight); //сигнал на оработку картинки
};

#endif // RAPIDTHREAD_H
