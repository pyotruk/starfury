#include "frame.h"
/////////////////////////////////////////////////////////////////////////////////////
Frame::Frame() :
    _data(new uchar[0])
{}
/////////////////////////////////////////////////////////////////////////////////////
Frame::Frame(const Frame &f) :
    _data(new uchar[0])
{
    this->setHeader(f.header());
    memcpy(this->_data, f._data, _dataSize);
    this->_timeMarker = f.timeMarker();
    this->cookCvMat();
}
/////////////////////////////////////////////////////////////////////////////////////
Frame::Frame(const quint32 width,
             const quint32 height,
             const quint32 depth) :
    _data(new uchar[0])
{
    this->setHeader(width,
                    height,
                    depth);
    this->fillZeros();
    this->cookCvMat();
}
/////////////////////////////////////////////////////////////////////////////////////
Frame::~Frame()
{
    delete []_data;
}
/////////////////////////////////////////////////////////////////////////////////////
Frame& Frame::operator =(const Frame &f)
{
    if(this != &f)
    {
        this->setHeader(f.header());
        memcpy(this->_data, f._data, _dataSize);
        this->_timeMarker = f.timeMarker();
        this->cookCvMat();
    }
    return *this;
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::setHeader(const quint32 width,
                      const quint32 height,
                      const quint32 depth)
{
    _header.width    = width;
    _header.height   = height;
    if(depth > __maxDepth)    _header.depth = __maxDepth;
    else                      _header.depth = depth;
    this->refreshDataSize();
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::setHeader(const Header &h)
{
    this->setHeader(h.width, h.height, h.depth);
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::copyFromRawData(const void *data,
                            const quint32 width,
                            const quint32 height,
                            const quint32 depth)
{
    this->setHeader(width,
                    height,
                    depth);
    const void *src = data;
    void *dst = _data;
    memcpy(dst, src, _dataSize);
    this->cookCvMat();
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::copyToQImage(QImage &img)
{
    img = QImage(_header.width,
                 _header.height,
                 QImage::Format_RGB32);
    uchar *ff = _data;
    QRgb *pLineStart, *pLineEnd;
    for(uint j = 0; j < _header.height; ++j)
    {
        pLineStart = (QRgb*)(img.scanLine(j));
        pLineEnd = pLineStart + _header.width;
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
    uchar *fin = _data + _dataSize + 1;
    for(; it < fin; ++it)
    {
        *it = 0;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::cookCvMat()
{
    this->_cvmat = cv::Mat(this->_header.height,
                           this->_header.width,
                           CV_8UC1,
                           this->_data);
}
/////////////////////////////////////////////////////////////////////////////////////
void Frame::refreshDataSize()
{
    _dataSize = _header.width * _header.height * _header.depth;
    delete []_data;
    _data = new uchar[_dataSize];
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//static members
/////////////////////////////////////////////////////////////////////////////////////
bool operator ==(const Frame::Header &h1,
                 const Frame::Header &h2)
{
    return ((h1.width  == h2.width)  &&
            (h1.height == h2.height) &&
            (h1.depth  == h2.depth));
}
/////////////////////////////////////////////////////////////////////////////////////
bool operator !=(const Frame::Header &h1,
                 const Frame::Header &h2)
{
    return ((h1.width  != h2.width)  ||
            (h1.height != h2.height) ||
            (h1.depth  != h2.depth));
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
