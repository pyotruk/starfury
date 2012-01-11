#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <QDateTime>
#include <QPointF>
#include <QRect>
#include <QObject>
/////////////////////////////////////////////////////////////////////////////////////
#include "opencv.hpp"
#include "boxes/frame.h"
#include "utils/cvhelpfun.h"
/////////////////////////////////////////////////////////////////////////////////////
class Accumulator : public QObject
{
    Q_OBJECT
public:
    static const int _defaultCapacity = 20;
    explicit Accumulator(const int capacity = _defaultCapacity) :
        _capacity(capacity), _num(0), _full(false) {}
    ~Accumulator() {}
    const FrameBox& frame() const {return _frame;}
    const FrameBox& add(FrameBox&);
    const FrameBox& shiftAndAdd(FrameBox&,
                                const QPointF& velocity);
    void setCapacity(const int capacity) {_capacity = capacity;}
    int capacity() const {return _capacity;}
    int num()      const {return _num;}
    bool isFull() const {return _full;}
    void clear();
private:
    static const double _alpha = 1.0;
    static const double _beta  = 1.0;
    FrameBox  _frame;
    int       _capacity; //ёмкость (кол-во накапливаемых кадров)
    int       _num;      //текущее кол-во накопленных кадров
    bool      _full;
    QDateTime _firstFrameTime;
    void checkSize(const Frame::Header&);
    void checkFull();
signals:
    void full();
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
namespace shifting
{
void calcShift(const QDateTime &t1,
               const QDateTime &t2,
               const QPointF   &velocity,
               QPoint &shift);
void cookShiftedFrame(const QPoint shift,
                      const double mean,
                      Frame&);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // ACCUMULATOR_H
