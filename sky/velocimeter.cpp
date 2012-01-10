#include "velocimeter.h"
/////////////////////////////////////////////////////////////////////////////////////
void Velocimeter::addPoint(const QPointF      &screenCenter,
                           const TelescopeBox &telescopePos,
                           const QSizeF       &field,
                           const QSize        &screen)
{
    if(_open2)
    {
        this->addSecondPoint(telescopePos,
                             field,
                             screen);
        this->calcVelocity();
    }

    if(_open1)
    {
        this->addFirstPoint(screenCenter,
                            telescopePos,
                            field,
                            screen);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Velocimeter::addFirstPoint(const QPointF      &screenCenter,
                                const TelescopeBox &telescopePos,
                                const QSizeF       &field,
                                const QSize        &screen)
{
    _c1 = screenCenter;
    _t1 = telescopePos.timeMarker();
    double screenAngleX, screenAngleY;
    ac::screenPoint2screenAngles(screenCenter.x(),
                                 screenCenter.y(),
                                 telescopePos.data().delta,
                                 field,
                                 screen,
                                 screenAngleX,
                                 screenAngleY);
    ac::screenAngles2iieqt(screenAngleX,
                           screenAngleY,
                           telescopePos.data().azHoriz,
                           telescopePos.data().elHoriz,
                           telescopePos.data().LST,
                           telescopePos.data().latitude,
                           _alpha,
                           _delta);
}
/////////////////////////////////////////////////////////////////////////////////////
void Velocimeter::addSecondPoint(const TelescopeBox &telescopePos,
                                 const QSizeF       &field,
                                 const QSize        &screen)
{
    double screenAngleX, screenAngleY;
    ac::iieqt2screenAngles(_alpha,
                           _delta,
                           telescopePos.data().LST,
                           telescopePos.data().latitude,
                           telescopePos.data().azHoriz,
                           telescopePos.data().elHoriz,
                           screenAngleX,
                           screenAngleY);
    double x, y;
    ac::screenAngles2screenPoint(screenAngleX,
                                 screenAngleY,
                                 telescopePos.data().delta,
                                 field,
                                 screen,
                                 x,
                                 y);
    _c2 = QPointF(x, y);
    _t2 = telescopePos.timeMarker();
}
/////////////////////////////////////////////////////////////////////////////////////
void Velocimeter::calcVelocity()
{
    int dt = _t1.time().msecsTo(_t2.time());
    qDebug() << "dt = " << dt;
    _vel.setX((_c2.x() - _c1.x()) / (dt / 1000.0));
    _vel.setY((_c2.y() - _c1.y()) / (dt / 1000.0));
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
