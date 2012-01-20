#include "velocimeter.h"
/////////////////////////////////////////////////////////////////////////////////////
void Velocimeter::addPoint(const QPointF &screenCenter,
                           const TelBox  &telPos,
                           const QSizeF  &field,
                           const QSize   &screen)
{
    ++_samples;
    if(_samples > _sampleStep)
    {
        _samples = 0;
        _open2 = true;
    }

    if(_open2)
    {
        _open2 = false;
        _open1 = true;
        this->addSecondPoint(telPos,
                             field,
                             screen);
        this->calcVelocity();
        emit velocityReady(_vel.x(), _vel.y());
    }

    if(_open1)
    {
        _open1 = false;
        this->addFirstPoint(screenCenter,
                            telPos,
                            field,
                            screen);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Velocimeter::addFirstPoint(const QPointF &screenCenter, // _c1
                                const TelBox  &telPos,
                                const QSizeF  &field,
                                const QSize   &screen)
{
    _c1 = screenCenter;
    _t1 = telPos.timeMarker();
    double screenAngleX, screenAngleY;
    ac::screenPoint2screenAngles(screenCenter.x(),
                                 screenCenter.y(),
                                 telPos.data().delta,
                                 field,
                                 screen,
                                 screenAngleX,
                                 screenAngleY);
    ac::screenAngles2iieqt(screenAngleX,
                           screenAngleY,
                           telPos.data().azHoriz,
                           telPos.data().elHoriz,
                           telPos.data().LST,
                           telPos.data().latitude,
                           _alpha,
                           _delta);
}
/////////////////////////////////////////////////////////////////////////////////////
void Velocimeter::addSecondPoint(const TelBox &telPos, // _c2
                                 const QSizeF &field,
                                 const QSize  &screen)
{
    double screenAngleX, screenAngleY;
    ac::iieqt2screenAngles(_alpha,
                           _delta,
                           telPos.data().LST,
                           telPos.data().latitude,
                           telPos.data().azHoriz,
                           telPos.data().elHoriz,
                           screenAngleX,
                           screenAngleY);
    double x, y;
    ac::screenAngles2screenPoint(screenAngleX,
                                 screenAngleY,
                                 telPos.data().delta,
                                 field,
                                 screen,
                                 x,
                                 y);
    _c2 = QPointF(x, y);
    _t2 = telPos.timeMarker();
}
/////////////////////////////////////////////////////////////////////////////////////
void Velocimeter::calcVelocity()
{
    int dt = _t1.time().msecsTo(_t2.time());
    double vx = (_c2.x() - _c1.x()) / (dt / 1000.0);
    double vy = (_c2.y() - _c1.y()) / (dt / 1000.0);
    _vel.setX(0 - vx); //т.к. нужна скорость объекта относительно звёзд
    _vel.setY(0 - vy);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
