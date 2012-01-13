#include "frame.h"
/////////////////////////////////////////////////////////////////////////////////////
void Frame::Header::setHeader(const int width,
                              const int height)
{
    _width  = width;
    _height = height;
}
/////////////////////////////////////////////////////////////////////////////////////
Frame::Header& Frame::Header::operator=(const Header &h)
{
    if(this != &h)
    {
        this->setHeader(h.width(), h.height());
    }
    return *this;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
Frame::Frame()
    : _data(new uchar[0])
{}
/////////////////////////////////////////////////////////////////////////////////////
Frame::~Frame()
{
    delete []_data;
}
/////////////////////////////////////////////////////////////////////////////////////
Frame::Frame(const Frame &f)
    : _data(new uchar[0])
{
    _header.setHeader(f.header());
    this->realloc(_header);
    memcpy(this->_data, f._data, _header.dataSize());
    this->cookCvMat();
}
/////////////////////////////////////////////////////////////////////////////////////
Frame::Frame(const int width,
             const int height)
    : _data(new uchar[0])
{
    _header.setHeader(width, height);
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
                                const int height)
{
    _header.setHeader(width, height);
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
                            const int height)
{
    _header.setHeader(width, height);
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
void Frame::cookCvMat()
{
    _cvmat = cv::Mat(_header.height(),
                     _header.width(),
                     CV_8UC1,
                     _data);
}
/////////////////////////////////////////////////////////////////////////////////////
uchar* Frame::pnt(int i, int j)
{
    return (this->_data + j * this->header().width() + i);
}
/////////////////////////////////////////////////////////////////////////////////////
const uchar* Frame::constPnt(int i, int j) const
{
    return (this->_data + j * this->header().width() + i);
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::fillZeros()
{
    this->fill(0);
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::fill(const uchar a)
{
    uchar *it = _data;
    uchar *fin = _data + _header.dataSize();
    for(; it < fin; ++it)
    {
        *it = a;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
bool Frame::rectIsBelongTo(const QRect &r)
{
    if((r.left() >= 0) && (r.top() >= 0))
    {
        if((r.right() < this->header().width()) && (r.bottom() < this->header().height()))
        {
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
bool Frame::pointIsBelongTo(const QPoint &p)
{
    if((p.x() >= 0) && (p.y() >= 0))
    {
        if((p.x() < this->header().width()) && (p.y() < this->header().height()))
        {
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
bool Frame::copyRegionTo(const QRect &region,
                         Frame &dst)
{
    if(!region.isValid())    return false;
    if(!this->rectIsBelongTo(region))    return false;
    dst.setHeaderAndRealloc(region.width(),
                            region.height());
    for(int j = 0; j < dst.header().height(); ++j)
    {
        for(int i = 0; i < dst.header().width(); ++i)
        {
            uchar* pDst = dst.pnt(i, j);
            const uchar* pSrc = this->constPnt(i + region.left(), j + region.top());
            *pDst = *pSrc;
        }
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////
bool Frame::copyRegionFrom(const Frame &src,
                           const QPoint &topLeft)
{
    QRect region(topLeft, QSize(src.header().width(),
                                src.header().height()));
    if(!region.isValid())    return false;
    if(!this->rectIsBelongTo(region))    return false;
    for(int j = 0; j < src.header().height(); ++j)
    {
        for(int i = 0; i < src.header().width(); ++i)
        {
            uchar *pDst= this->pnt(i + region.left(), j + region.top());
            const uchar *pSrc = src.constPnt(i, j);
            *pDst = *pSrc;
        }
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
bool operator ==(const Frame::Header &h1,
                 const Frame::Header &h2)
{
    return ((h1.width()  == h2.width()) && (h1.height() == h2.height()));
}
/////////////////////////////////////////////////////////////////////////////////////
bool operator !=(const Frame::Header &h1,
                 const Frame::Header &h2)
{
    return ((h1.width()  != h2.width()) || (h1.height() != h2.height()));
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
QDebug operator<<(QDebug d, const Frame::Header &h)
{
    d << "     width  = " << h.width() << "\n"
      << "     height = " << h.height();
    return d;
}
/////////////////////////////////////////////////////////////////////////////////////
QDebug operator<<(QDebug d, const Frame &f)
{
    d << "Frame: " << f.data() << "\n"
      << f.header();
    return d;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
