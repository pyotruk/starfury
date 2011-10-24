#ifndef RAPIDTHREAD_H
#define RAPIDTHREAD_H

#include <QThread>
#include <QSettings>
#include <QSize>
#include "readsharedmem.h"

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
    QSize       FFrameSize;
    bool checkFrameSize(const FrameHeader &header,
                        const QSize       &frameSize);
signals:
    void frameReceived(void *pFrame,
                       int  frameWidth,
                       int  frameHeight); //сигнал на оработку картинки
};

#endif // RAPIDTHREAD_H
