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
    explicit DoubleBuffer(const qint64 &singleBufSize = DEFAULT_SINGLE_DATA_SIZE);
    ~DoubleBuffer();
    void switchBuffers();
    SingleBuffer *getRapidBuf();
    SingleBuffer *getSlowBuf();
    void setSize(const qint64 &singleBufSize);
private:
    static const int DEFAULT_SINGLE_DATA_SIZE = 307200;
    qint64 FSize;
    SingleBuffer *FRapidBuf;
    SingleBuffer *FSlowBuf;
private slots:
    void unlockSlowBuf();
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // DOUBLEBUFFER_H
