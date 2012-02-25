/* ����� ��������� ���������� ������, � ����� ���������� ������ �� �������.
   �������� ������ ����������� (shifting::) � ����������� �� �������� ��������
   (��������� � ����� Accumulator::add()) */
#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <QDateTime>
#include <QVector2D>
#include <QRect>
#include <QSize>
#include <qmath.h>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/frame.h"
#include "utils/cvwrap.h"
/////////////////////////////////////////////////////////////////////////////////////
class Accumulator
{
public:
    static const int _defaultCapacity = 20;
    explicit Accumulator(const int capacity = _defaultCapacity) :
        _capacity(capacity), _num(0), _full(false) {}
    ~Accumulator() {}
    const FrameBox& add(FrameBox&,
                        const QVector2D& velocity = QVector2D(0, 0));
    void setCapacity(const int capacity) {_capacity = capacity;}
    inline const FrameBox& frame() const {return _frame;}
    inline int  capacity() const {return _capacity;}
    inline int  num()      const {return _num;}
    inline bool isFull()   const {return _full;}
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
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
namespace shifting
{
/* returns true if success */
bool cookShiftedFrame(const QDateTime &t1, //������ ���� � ����� ����������
                      const QDateTime &t2, //��������� ����
                      const QVector2D &velocity,
                      const double mean,
                      Frame &f);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // ACCUMULATOR_H
