#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
Strob::Geometry::Geometry() :
    _frameSize(_defaultFrameWidth, _defaultFrameHeight),
    _refPoint(_defaultRefPointX, _defaultRefPointY),
    _velocity(_defaultVelocityX, _defaultVelocityY)
{
    this->moveToRefPoint();
    this->setSide(_defaultSide);
}
/////////////////////////////////////////////////////////////////////////////////////
const QPoint& Strob::Geometry::center()
{
    _cache_Center = _signal.center();
    return _cache_Center;
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::setSide(const int s)
{
    Geometry buf = *this; //backup
    _signal.setSize(QSize(s, s));
    this->refreshFoneRect();    
    if(!this->isValid())    *this = buf; //rollback
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::setCenter(const QPoint &p)
{
    Geometry buf = *this; //backup
    _signal.moveCenter(p);
    this->refreshFoneRect();
    if(!this->isValid())    *this = buf; //rollback
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::setRefPoint(const QPoint &p)
{
    _refPoint = p;
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::setVelocity(const QPointF &v)
{
    _velocity = QVector2D(v);
    this->refreshFoneRect();
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::refreshFoneRect()
{
    int r = _signal.width();
    QVector2D v = _velocity;
    v.normalize();
    _fone.moveCenter(QPoint((int)(_signal.center().x() + r * v.x()),
                            (int)(_signal.center().y() + r * v.y())));
    _fone.setSize(_signal.size());
}
/////////////////////////////////////////////////////////////////////////////////////
const cv::Rect& Strob::Geometry::signalCvRect()
{
    cvhelp::qtRect2cvRect(_signal, _cache_Signal);
    return _cache_Signal;
}
/////////////////////////////////////////////////////////////////////////////////////
const cv::Rect& Strob::Geometry::foneCvRect()
{
    cvhelp::qtRect2cvRect(_fone, _cache_Fone);
    return _cache_Fone;
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::setFrameSize(const QSize &s)
{
    _frameSize = s;
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::moveToRefPoint()
{
    this->setCenter(_refPoint);
}
/////////////////////////////////////////////////////////////////////////////////////
bool Strob::Geometry::isValid()
{
    QRect frameRect(QPoint(0, 0), _frameSize);
    if(frameRect.contains(_signal, true))
    {
        if(frameRect.contains(_fone, true))
        {
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
