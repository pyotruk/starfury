/* Класс для работы с видеокадром
   1 байт на пиксель */
#ifndef FRAME_H
#define FRAME_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <QtGlobal>
#include <QImage>
#include <QRect>
#include <QPoint>
#include <QSize>
/////////////////////////////////////////////////////////////////////////////////////
#include "opencv.hpp"
#include "boxes/databox.h"
/////////////////////////////////////////////////////////////////////////////////////
class Frame
{
public:
    class Header
    {
    public:
        explicit Header(const int width = 0,
                        const int height = 0)
            {this->setHeader(width, height);}
        explicit Header(const Header &h)
            {this->setHeader(h.width(), h.height());}
        Header& operator=(const Header &h);
        inline int width()  const {return _width;}
        inline int height() const {return _height;}
        void setHeader(const int width,
                       const int height);
        void setHeader(const Header &h)
            {this->setHeader(h.width(), h.height());}
        inline quint64 dataSize() const {return _width * _height;}
    private:
        int _width;
        int _height;
    };

    explicit Frame();
    explicit Frame(const Frame&);
    explicit Frame(const int width,
                   const int height);
    ~Frame();
    Frame& operator=(const Frame&);
    inline const Header& header() const {return _header;}
    inline const uchar* data() const {return _data;}
    inline cv::Mat& asCvMat()        {return _cvmat;}
    inline const cv::Mat& asCvMat() const {return _cvmat;}
    void setHeaderAndRealloc(const int width,      //data size changes too
                             const int height);
    void setHeaderAndRealloc(const Header&);
    void copyFromRawData(const void *data,
                         const int width,
                         const int height);
    void copyToQImage(QImage&);
    uchar* pnt(int i, int j);
    const uchar* constPnt(int i, int j) const;
    void fillZeros();
    void fill(const uchar);
    bool rectIsBelongTo(const QRect&);
    bool pointIsBelongTo(const QPoint&);
    bool copyRegionTo(const QRect& region, //returns true if success
                      Frame& dst);
    bool copyRegionFrom(const Frame &src,  //returns true if success
                        const QPoint &topLeft);
private:
    uchar         *_data;
    Header         _header;
    cv::Mat        _cvmat;
    void realloc(const Header&);
    void cookCvMat();
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
bool operator ==(const Frame::Header &h1,
                 const Frame::Header &h2);
bool operator !=(const Frame::Header &h1,
                 const Frame::Header &h2);
/////////////////////////////////////////////////////////////////////////////////////
QDebug operator<<(QDebug, const Frame::Header&);
QDebug operator<<(QDebug, const Frame&);
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
typedef DataBox<Frame> FrameBox;
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

#endif // FRAME_H
