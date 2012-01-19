#include "accumulator.h"
/////////////////////////////////////////////////////////////////////////////////////
const FrameBox& Accumulator::add(FrameBox &f,
                                 const QPointF &velocity)
{
    //qDebug() << "Accumulator: add(),  num = " << _num << "  velocity = " << velocity;
    if(this->isFull())    return _frame;
    if(_num == 0)
    {
        _frame.setTimeMarker(f.timeMarker());
        //qDebug() << "Accumulator: first frame time =" << _frame.timeMarker();
    }
    this->checkSize(f.data().header());
    double mean = cv::mean(f.data().asCvMat())[0];
    if(!shifting::cookShiftedFrame(_frame.timeMarker(),
                                   f.timeMarker(),
                                   velocity,
                                   mean,
                                   f.data()))
    {
        this->setFull();
        return _frame;
    }
    cv::addWeighted(f.data().asCvMat(),
                    _alpha,
                    _frame.data().asCvMat(),
                    _beta,
                    0 - mean,
                    _frame.data().asCvMat());
    this->checkFull();
    return _frame;
}
/////////////////////////////////////////////////////////////////////////////////////
void Accumulator::checkSize(const Frame::Header &newHeader)
{
    if(_frame.data().header() != newHeader)
    {
        _frame.data().setHeaderAndRealloc(newHeader);
        _frame.data().fillZeros();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Accumulator::checkFull()
{
    ++_num;
    if(_num > _capacity)
    {
        this->setFull();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Accumulator::clear()
{
    _full = false;
    _num = 0;
    _frame.data().fillZeros();
    qDebug() << "Accumulator: i`m CLEAR !";
}
/////////////////////////////////////////////////////////////////////////////////////
void Accumulator::setFull()
{
    _full = true;
    qDebug() << "Accumulator: i`m FULL !" << "\n"
             << "    num = " << _num << "    capacity = " << _capacity;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
bool shifting::cookShiftedFrame(const QDateTime &t1, //первый кадр в серии накоплени€
                                const QDateTime &t2, //сдвинутый кадр
                                const QPointF &velocity,
                                const double mean,
                                Frame &f)
{
    //вычисление сдвига
    int dt = t1.time().msecsTo(t2.time());
    QPoint shift((int)(velocity.x() * dt / 1000.0),
                 (int)(velocity.y() * dt / 1000.0));

    /*вычисление пр€моугольника, образованного пересечением
      первого кадра (в серии накоплени€) и сдвинутого кадра*/
    QRect rect0(QPoint(0, 0),
                QPoint(f.header().width() - 1,
                       f.header().height() - 1));
    QRect rect1(rect0);
    rect1.moveCenter(QPoint(rect0.center().x() + shift.x(),
                            rect0.center().y() + shift.y()));
    QRect initialRegion = rect0.intersected(rect1); //в —  первого кадра
    QRect rect2(rect0);
    rect2.moveCenter(QPoint(rect0.center().x() - shift.x(),
                            rect0.center().y() - shift.y()));
    QRect shiftedRegion = rect0.intersected(rect2); //в —  сдвинутого кадра

    Frame intersectedFrame;
    if(!f.copyRegionTo(shiftedRegion, intersectedFrame))
    {
        qDebug() << "cookShiftedFrame(): Frame::copyRegionTo FAILED" << "\n"
                 << "   " << shiftedRegion;
        return false;
    }
    f.fill(qCeil(mean));
    if(!f.copyRegionFrom(intersectedFrame, initialRegion.topLeft()))
    {
        qDebug() << "cookShiftedFrame(): Frame::copyRegionFrom FAILED" << "\n"
                 << "   " << initialRegion;
        return false;
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
