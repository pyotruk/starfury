#ifndef FRAME_H
#define FRAME_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QtGlobal>
#include <QReadWriteLock>
#include <string.h>
#include <QImage>
#include <QDebug>
#include "opencv.hpp"
/////////////////////////////////////////////////////////////////////////////////////
struct FrameHeader
{
    qint64  timeID;
    quint32 frameID;
    quint32 dataSize; // = depth * width * height
    quint32 depth;
    quint32 width;
    quint32 height;
    quint32 plug;
};
/////////////////////////////////////////////////////////////////////////////////////
class Frame
{
public:
    explicit Frame();
    explicit Frame(const Frame&);
    ~Frame();
    Frame& operator =(const Frame&);
    void attachRawData(const FrameHeader &header,
                       const void *data);
    void unattachRawData(); //do not forget!
    uchar* data()               const {return _data;}
    const FrameHeader& header() const {return _header;}
    QReadWriteLock& lock()      {return _lock;}
    cv::Mat& asCvMat()          {return _cvmat;}
    void copyToQImage(QImage&);
private:
    uchar         *_data;
    FrameHeader    _header;
    QReadWriteLock _lock;
    cv::Mat        _cvmat;
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

#endif // FRAME_H
