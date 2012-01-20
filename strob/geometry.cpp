#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
Strob::Geometry::Geometry() :
    _isValid(false),
    _autoRefresh(false)
{
    this->setFrameSize(QSize(_defaultFrameWidth, _defaultFrameHeight));
    this->setVelocity(QPointF(_defaultVelocityX, _defaultVelocityY));
    this->setSide(_defaultSide);
    this->setRefPoint(QPoint(_defaultRefPointX, _defaultRefPointY));
    this->moveToRefPoint();
    this->refresh();
}
/////////////////////////////////////////////////////////////////////////////////////
const QPoint& Strob::Geometry::center()
{
    _cache_Center = _signal.center();
    return _cache_Center;
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::verifSide(int &s)
{
    if(s < _minSide)    s = _minSide;
    if(s > _maxSide)    s = _maxSide;
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::setSide(const int s)
{
    int side = s;
    this->verifSide(side);
    _signal.setSize(QSize(side, side));
    if(_autoRefresh)    this->refresh();
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::setCenter(const QPoint &p)
{
    _signal.moveCenter(p);
    if(_autoRefresh)    this->refresh();
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
    _velocity.normalize();
    if(_autoRefresh)    this->refresh();
}
/////////////////////////////////////////////////////////////////////////////////////
const cv::Rect& Strob::Geometry::signalCvRect()
{
    cvwrap::qtRect2cvRect(_signal, _cache_Signal);
    return _cache_Signal;
}
/////////////////////////////////////////////////////////////////////////////////////
const cv::Rect& Strob::Geometry::foneCvRect()
{
    cvwrap::qtRect2cvRect(_fone, _cache_Fone);
    return _cache_Fone;
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::verifFrameSize(QSize &s)
{
    if(s.width()  < _minFrameWidth)     s.setWidth(_minFrameWidth);
    if(s.height() < _minFrameHeight)    s.setHeight(_minFrameHeight);
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::setFrameSize(const QSize &s)
{
    QSize size = s;
    this->verifFrameSize(size);
    _frameSize = size;
    if(_autoRefresh)    this->refresh();
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::moveToRefPoint()
{
    this->setCenter(_refPoint);
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::moveToFrameCenter()
{
    this->setCenter(QPoint(_frameSize.width() / 2,
                           _frameSize.height() / 2));
}
/////////////////////////////////////////////////////////////////////////////////////
bool Strob::Geometry::checkValid()
{
    QRect frameRect(QPoint(0, 0), _frameSize);
    if(frameRect.contains(_fone, true))
    {
        if(frameRect.contains(_signal, true))
        {
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::backup()
{
    _backup_Signal = _signal;
    _backup_Fone   = _fone;
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::rollback()
{
    _signal = _backup_Signal;
    _fone   = _backup_Fone;
    this->refreshValid();
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::refresh()
{
    this->refreshFoneRect();
    this->refreshValid();

    _autoRefresh = false; //чтобы не было рекурсии
    if(_isValid) //если тебе хорошо
    {
        this->backup(); //запомни это состояние
    }
    else                //если тебе плохо
    {
        this->rollback();   //вернись в то состояние, когда было хорошо
        if(!_isValid)       //если тебе всё ещё плохо
        {
            this->moveToRefPoint(); //вернись в то место, где тебе было хорошо
            if(!_isValid)           //не помогает ?
            {
                this->moveToFrameCenter();      //встань в центр кадра, детка
            }
        }
    }
    _autoRefresh = true;
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::refreshFoneRect()
{
    int r = _signal.width() * _foneRectSideKoef;
    _fone.moveCenter(QPoint((int)(_signal.center().x() + r * _velocity.x()),
                            (int)(_signal.center().y() + r * _velocity.y())));
    _fone.setSize(QSize(r, r));
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::refreshValid()
{
     _isValid = this->checkValid();
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
