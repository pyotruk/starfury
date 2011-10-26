#ifndef DOUBLEBUFFER_H
#define DOUBLEBUFFER_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QObject>
#include "singlebuffer.h"
/////////////////////////////////////////////////////////////////////////////////////
class DoubleBuffer : public QObject
{
    Q_OBJECT
public:
    explicit DoubleBuffer(const qint64 singleBufSize = _defaultSingleDataSize);
    ~DoubleBuffer();
    void switchBuffers();
    SingleBuffer *getRapidBuf();
    SingleBuffer *getSlowBuf();
    void setSize(const qint64 singleBufSize);
private:
    static const int _defaultSingleDataSize = 307200;
    qint64 _size;
    SingleBuffer *_rapidBuf;
    SingleBuffer *_slowBuf;
private slots:
    void unlockSlowBuf();
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // DOUBLEBUFFER_H
