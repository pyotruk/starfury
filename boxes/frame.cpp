#include "frame.h"
/////////////////////////////////////////////////////////////////////////////////////
void Frame::Header::setHeader(const int width,
                              const int height,
                              const int depth)
{
    _width  = width;
    _height = height;
    if(depth > __maxDepth)    _depth = __maxDepth;
    else                      _depth = depth;
}
/////////////////////////////////////////////////////////////////////////////////////
Frame::Header& Frame::Header::operator=(const Header &h)
{
    if(this != &h)
    {
        this->setHeader(h.width(), h.height(), h.depth());
    }
    return *this;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
Frame::Frame(const Frame &f) :
    _data(new uchar[0])
{
    _header.setHeader(f.header());
    this->realloc(_header);
    memcpy(this->_data, f._data, _header.dataSize());
    this->_timeMarker = f.timeMarker();
    this->cookCvMat();
}
/////////////////////////////////////////////////////////////////////////////////////
Frame::Frame(const int width,
             const int height,
             const int depth) :
    _data(new uchar[0])
{
    _header.setHeader(width, height, depth);
    this->realloc(_header);
    this->fillZeros();
    this->cookCvMat();
}
/////////////////////////////////////////////////////////////////////////////////////
Frame& Frame::operator =(const Frame &f)
{
    if(this != &f)
    {
        _header.setHeader(f.header());
        this->realloc(_header);
        memcpy(this->_data, f._data, _header.dataSize());
        this->_timeMarker = f.timeMarker();
        this->cookCvMat();
    }
    return *this;
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::realloc(const Header &h)
{
    delete []_data;
    _data = new uchar[h.dataSize()];
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::setHeaderAndRealloc(const int width,
                                const int height,
                                const int depth)
{
    _header.setHeader(width, height, depth);
    this->realloc(_header);
    this->cookCvMat();
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::setHeaderAndRealloc(const Header &h)
{
    _header.setHeader(h);
    this->realloc(_header);
    this->cookCvMat();
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::copyFromRawData(const void *data,
                            const int width,
                            const int height,
                            const int depth)
{
    _header.setHeader(width, height, depth);
    this->realloc(_header);
    const void *src = data;
    void *dst = _data;
    memcpy(dst, src, _header.dataSize());
    this->cookCvMat();
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::copyToQImage(QImage &img)
{
    img = QImage(_header.width(),
                 _header.height(),
                 QImage::Format_RGB32);
    uchar *ff = _data;
    QRgb *pLineStart, *pLineEnd;
    for(int j = 0; j < _header.height(); ++j)
    {
        pLineStart = (QRgb*)(img.scanLine(j));
        pLineEnd = pLineStart + _header.width();
        for(QRgb *pline = pLineStart; pline < pLineEnd; ++pline)
        {
            *pline = qRgba(*ff, *ff, *ff, 255);
            ++ff;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::fillZeros()
{
    uchar *it = _data;
    uchar *fin = _data + _header.dataSize();
    for(; it < fin; ++it)
    {
        *it = 0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::cookCvMat()
{
    _cvmat = cv::Mat(_header.height(),
                     _header.width(),
                     CV_8UC1,
                     _data);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
bool operator ==(const Frame::Header &h1,
                 const Frame::Header &h2)
{
    return ((h1.width()  == h2.width())  &&
            (h1.height() == h2.height()) &&
            (h1.depth()  == h2.depth()));
}
/////////////////////////////////////////////////////////////////////////////////////
bool operator !=(const Frame::Header &h1,
                 const Frame::Header &h2)
{
    return ((h1.width()  != h2.width())  ||
            (h1.height() != h2.height()) ||
            (h1.depth()  != h2.depth()));
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
QDebug operator<<(QDebug d, const Frame::Header &h)
{
    d << "     width  = " << h.width() << "\n"
      << "     height = " << h.height() << "\n"
      << "     depth  = " << h.depth();
    return d;
}
/////////////////////////////////////////////////////////////////////////////////////
QDebug operator<<(QDebug d, const Frame &f)
{
    d << "Frame: " << f.data() << "\n"
      << f.header() << "\n"
      << "time marker: " << f.timeMarker();
    return d;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
