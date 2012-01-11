#ifndef VELOCIMETER_H
#define VELOCIMETER_H
/////////////////////////////////////////////////////////////////////////////////////
/* ����� �������� �������� �������� �� ���� ������.
   ���� ������:
   _1_ � ������ ������� t1 ����������� ���������� (alpha, delta) �������������� ������,
       ����������� � ������ ����� (x1, y1)
   _2_ � ������ ������� t2 ����������� alpha, delta ����������� � �������� �� (x2, y2)
   _3_ vx = dx / dt,  vy = dy / dt
*/
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <QPointF>
#include <QSizeF>
#include <QDateTime>
#include <QObject>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/telescope.h"
#include "math/astrocalc.h"
/////////////////////////////////////////////////////////////////////////////////////
class Velocimeter : public QObject
{
    Q_OBJECT
public:
    explicit Velocimeter()
        : _open1(true), _open2(false), _samples(0), _alpha(0), _delta(0), _vel(0, 0) {}
    void addPoint(const QPointF      &screenCenter,
                  const TelescopeBox &telescopePos,
                  const QSizeF       &field,
                  const QSize        &screen);
    const QPointF& vel() const {return _vel;}
private:
    Velocimeter(const Velocimeter&) {}
    Velocimeter& operator =(const Velocimeter&) {return *this;}
    static const int _sampleStep = 30; //���������� ����� ����� ������� (_c1, _c2) �� ������� � ������� (_samples)
    bool      _open1;
    bool      _open2;
    int       _samples; //���-�� ������� ������� addPoint()
    QPointF   _c1;
    QPointF   _c2;
    QDateTime _t1;
    QDateTime _t2;
    double    _alpha;
    double    _delta;
    QPointF   _vel;
    void addFirstPoint(const QPointF      &screenCenter, // _c1
                       const TelescopeBox &telescopePos,
                       const QSizeF       &field,
                       const QSize        &screen);
    void addSecondPoint(const TelescopeBox &telescopePos, // _c2
                        const QSizeF       &field,
                        const QSize        &screen);
    void calcVelocity();
signals:
    void velocityReady(const double vx,
                       const double vy);
};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // VELOCIMETER_H
