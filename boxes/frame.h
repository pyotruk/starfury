#ifndef FRAME_H
#define FRAME_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QtGlobal>
#include <QReadWriteLock>
#include <QImage>
#include <QDateTime>
/////////////////////////////////////////////////////////////////////////////////////
#include "opencv.hpp"
/////////////////////////////////////////////////////////////////////////////////////
class Frame
{
public:
    struct Header
    {
        quint32 depth;    //bytes per pixel
        quint32 width;
        quint32 height;
    };

    explicit Frame();
    explicit Frame(const Frame&);
    explicit Frame(const quint32 width,
                   const quint32 height,
                   const quint32 depth = 1); //bytes per pixel
    ~Frame();
    Frame& operator =(const Frame&);
    uchar* data()          const {return _data;}
    const Header& header() const {return _header;}
    QReadWriteLock& lock() {return _lock;}
    const QDateTime& timeMarker() const {return _timeMarker;}
    cv::Mat& asCvMat()     {return _cvmat;}
    void setHeader(const quint32 width,      //data size changes too
                   const quint32 height,
                   const quint32 depth = 1); //bytes per pixel
    void setHeader(const Header&);
    void setTimeMarker(const QDateTime &t) {_timeMarker = t;}
    void copyFromRawData(const void *data,
                         const quint32 width,
                         const quint32 height,
                         const quint32 depth = 1); //bytes per pixel
    void copyToQImage(QImage&);
    void fillZeros();
    void refreshDataSize();

    static const quint32 __maxDepth = 2;
private:
    uchar         *_data;
    quint64        _dataSize; //[bytes] = depth * width * height
    Header         _header;
    QReadWriteLock _lock;
    QDateTime      _timeMarker;
    cv::Mat        _cvmat;
    void cookCvMat();
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
bool operator ==(const Frame::Header &h1,
                 const Frame::Header &h2);
bool operator !=(const Frame::Header &h1,
                 const Frame::Header &h2);
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

#endif // FRAME_H
