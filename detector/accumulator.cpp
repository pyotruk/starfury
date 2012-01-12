#include "accumulator.h"
/////////////////////////////////////////////////////////////////////////////////////
const FrameBox& Accumulator::add(FrameBox &f)
{
    this->checkSize(f.data().header());
    double mean = cv::mean(f.data().asCvMat())[0];
    cv::addWeighted(f.data().asCvMat(),
                    _alpha,
                    _frame.data().asCvMat(),
                    _beta,
                    0 - mean,
                    _frame.data().asCvMat());
    _frame.setTimeMarker(f.timeMarker());
    this->checkFull();
    return _frame;
}
/////////////////////////////////////////////////////////////////////////////////////
const FrameBox& Accumulator::shiftAndAdd(FrameBox &f,
                                         const QPointF &velocity)
{
    if(_num == 0)
    {
        _firstFrameTime = f.timeMarker();
        qDebug() << "first frame time: " << _firstFrameTime;
    }
    this->checkSize(f.data().header());
    double mean = cv::mean(f.data().asCvMat())[0];
    f.data().asCvMat() -= mean;

    shifting::cookShiftedFrame(_firstFrameTime,
                               f.timeMarker(),
                               velocity,
                               f.data());
    cv::addWeighted(f.data().asCvMat(),
                    _alpha,
                    _frame.data().asCvMat(),
                    _beta,
                    0,
                    _frame.data().asCvMat());
    _frame.setTimeMarker(f.timeMarker());
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
        _full = true;
        emit full();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Accumulator::clear()
{
    _full = false;
    _num = 0;
    _frame.data().fillZeros();
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void shifting::cookShiftedFrame(const QDateTime &t1, //первый кадр в серии накоплени€
                                const QDateTime &t2, //сдвинутый кадр
                                const QPointF &velocity,
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
        qDebug() << "Frame::copyRegionTo FAILED" << "\n"
                 << "   " << shiftedRegion;
        return;
    }
    f.fillZeros();
    if(!f.copyRegionFrom(intersectedFrame, initialRegion.topLeft()))
    {
        qDebug() << "Frame::copyRegionFrom FAILED" << "\n"
                 << "   " << initialRegion;
        return;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
