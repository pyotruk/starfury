#ifndef DOUBLEBUFFER_H
#define DOUBLEBUFFER_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QObject>
#include "frame.h"
/////////////////////////////////////////////////////////////////////////////////////
class DoubleBuffer : public QObject
{
    Q_OBJECT
public:
    explicit DoubleBuffer();
    ~DoubleBuffer();
    void switchBuffers();
    Frame *rapidBuf();
    Frame *slowBuf();
private:
    Frame *_rapidBuf;
    Frame *_slowBuf;
private slots:
    void unlockSlowBuf();
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // DOUBLEBUFFER_H
