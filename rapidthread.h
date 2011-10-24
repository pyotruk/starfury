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
    explicit RapidThread(const int frameHeaderSize,
                         const int frameWidth,
                         const int frameHeight,
                         QSettings *settings = 0);
    ~RapidThread();
protected:
    void run();
private:
    SharedMem *FSharedMem;
    uchar     *FFrame;
signals:
    void frameReceived(void *pFrame,
                       int  frameWidth,
                       int  frameHeight); //сигнал на оработку картинки
};

#endif // RAPIDTHREAD_H
