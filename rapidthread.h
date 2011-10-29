#ifndef RAPIDTHREAD_H
#define RAPIDTHREAD_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QSettings>
#include <QSize>
#include <QTime>
#include <QDebug>
#include "readsharedmem.h"
#include "doublebuffer.h"
#include "frame.h"
/////////////////////////////////////////////////////////////////////////////////////
class RapidThread : public QThread
{
    Q_OBJECT
public:
    explicit RapidThread(QSettings *settings = 0);
    ~RapidThread();
    DoubleBuffer *doubleBuf();
protected:
    void run();
private:
    DoubleBuffer *_doubleBuf;
    SharedMem    *_sharedMem;
    FrameHeader  _frameHeader;
signals:
    void frame4Strob(Frame *frame); //слежение (строб)
    void frame4Gui(Frame *frame); //отображение (gui)
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // RAPIDTHREAD_H
