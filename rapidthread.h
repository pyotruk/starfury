#ifndef RAPIDTHREAD_H
#define RAPIDTHREAD_H

#include <QThread>
#include <QSettings>
#include <QSize>
#include <QTime>
#include <QDebug>
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
    static const int _defaultFrameWidth  = 640;
    static const int _defaultFrameHeight = 480;
    QSize        _frameSize;
    DoubleBuffer *_doubleBuf;
    SharedMem    *_sharedMem;
    FrameHeader  _frameHeader;
signals:
    void frame4Strob(void *frame,
                     const int frameWidth,
                     const int frameHeight); //слежение (строб)
    void frame4Gui(const void *frame,
                   const int  frameWidth,
                   const int  frameHeight); //отображение (gui)
};

#endif // RAPIDTHREAD_H
