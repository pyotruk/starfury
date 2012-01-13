#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <QDateTime>
#include <QPointF>
#include <QRect>
#include <QSize>
#include <QObject>
#include <qmath.h>
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
    const FrameBox& add(FrameBox&,
                        const QPointF& velocity = QPointF(0, 0));
    void setCapacity(const int capacity) {_capacity = capacity;}
    int capacity() const {return _capacity;}
    int num()      const {return _num;}
    bool isFull() const {return _full;}
    void clear();
private:
    static const double _alpha = 1.0;
    static const double _beta  = 1.0;
    FrameBox  _frame;
    int       _capacity; //������� (���-�� ������������� ������)
    int       _num;      //������� ���-�� ����������� ������
    bool      _full;
    void checkSize(const Frame::Header&);
    void checkFull();
    void setFull();
signals:
    void full();
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
namespace shifting
{
/* returns true if success */
bool cookShiftedFrame(const QDateTime &t1, //������ ���� � ����� ����������
                      const QDateTime &t2, //��������� ����
                      const QPointF &velocity,
                      const double mean,
                      Frame &f);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // ACCUMULATOR_H
