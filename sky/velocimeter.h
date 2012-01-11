#ifndef VELOCIMETER_H
#define VELOCIMETER_H
/////////////////////////////////////////////////////////////////////////////////////
/* Класс измеряет экранную скорость по двум точкам.
   Суть метода:
   _1_ в момент времени t1 вычисляются координаты (alpha, delta) гипотетической звезды,
       находящейся в центре кадра (x1, y1)
   _2_ в момент времени t2 вычисленные alpha, delta переводятся в экранную СК (x2, y2)
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
    static const int _sampleStep = 30; //расстояние между двумя точками (_c1, _c2) по времени в сэмплах (_samples)
    bool      _open1;
    bool      _open2;
    int       _samples; //кол-во вызовов функции addPoint()
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
