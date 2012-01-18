#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
Strob::Geometry::Geometry() :
    _frameSize(_defaultFrameWidth, _defaultFrameHeight),
    _refPoint(_defaultRefPointX, _defaultRefPointY),
    _velocity(_defaultVelocityX, _defaultVelocityY),
    _rollback_ON(false),
    _isValid(false)
{
    this->setSide(_defaultSide);
    _rollback_ON = true;
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
    int side;
    if(s < _minSide)    side = _minSide;
    else                side = s;
    _signal.setSize(QSize(side, side));
    this->refresh();
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::setCenter(const QPoint &p)
{
    _signal.moveCenter(p);
    this->refresh();
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
    this->refresh();
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::moveToRefPoint()
{
    this->setCenter(_refPoint);
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::moveToFrameCenter()
{
    this->setCenter(QPoint(_frameSize.width(),
                           _frameSize.height()));
}
/////////////////////////////////////////////////////////////////////////////////////
bool Strob::Geometry::checkValid()
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
void Strob::Geometry::backup()
{
    _clone_Signal = _signal;
    _clone_Fone   = _fone;
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::rollback()
{
    if(_rollback_ON)
    {
        _signal = _clone_Signal;
        _fone   = _clone_Fone;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::Geometry::refresh()
{
    this->refreshFoneRect();
    this->refreshValid();

    /* Achtung! Brainfuck */
    if(this->isValid()) //если тебе хорошо
    {
        this->backup(); //запомни это состояние
    }
    else                //если тебе плохо
    {
        this->rollback();       //вернись в то состояние, когда было хорошо
        if(!this->isValid())    //если тебе всё ещё плохо
        {
            this->moveToRefPoint();     //вернись в то место, где тебе было хорошо
            if(!this->isValid())        //не помогает ?
            {
                this->moveToFrameCenter();      //встань в центр кадра, детка
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
