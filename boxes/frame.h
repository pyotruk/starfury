#ifndef FRAME_H
#define FRAME_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <QtGlobal>
#include <QImage>
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
                        const int height = 0,
                        const int depth = 1)
            {this->setHeader(width, height, depth);}
        explicit Header(const Header &h)
            {this->setHeader(h.width(), h.height(), h.depth());}
        Header& operator=(const Header &h);
        int width()  const {return _width;}
        int height() const {return _height;}
        int depth()  const {return _depth;}
        void setHeader(const int width,
                       const int height,
                       const int depth = 1);
        void setHeader(const Header &h)
            {this->setHeader(h.width(), h.height(), h.depth());}
        quint64 dataSize() const {return _width * _height * _depth;}
    private:
        static const int __maxDepth = 2;
        int _width;
        int _height;
        int _depth;    //bytes per pixel
    };

    explicit Frame() : _data(new uchar[0]) {}
    explicit Frame(const Frame&);
    explicit Frame(const int width,
                   const int height,
                   const int depth = 1); //bytes per pixel
    ~Frame() {delete []_data;}
    Frame& operator =(const Frame&);
    uchar* data()          const {return _data;}
    const Header& header() const {return _header;}
    cv::Mat& asCvMat()             {return _cvmat;}
    const cv::Mat& asCvMat() const {return _cvmat;}
    void setHeaderAndRealloc(const int width,      //data size changes too
                             const int height,
                             const int depth = 1); //bytes per pixel
    void setHeaderAndRealloc(const Header&);
    void copyFromRawData(const void *data,
                         const int width,
                         const int height,
                         const int depth = 1); //bytes per pixel
    void copyToQImage(QImage&);
    void fillZeros();
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
