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

    QPoint shift;
    shifting::calcShift(_firstFrameTime,
                        f.timeMarker(),
                        velocity,
                        shift);
    shifting::cookShiftedFrame(shift, mean, f.data());
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
void shifting::calcShift(const QDateTime &t1,
                         const QDateTime &t2,
                         const QPointF &velocity,
                         QPoint &shift)
{
    int dt = t1.time().msecsTo(t2.time());
    shift.setX((int)(velocity.x() * dt / 1000.0));
    shift.setY((int)(velocity.y() * dt / 1000.0));
}
/////////////////////////////////////////////////////////////////////////////////////
void shifting::cookShiftedFrame(const QPoint shift,
                                const double mean,
                                Frame &f)
{
    QRect qtRect(shift,
                 QPoint(f.header().width() - 1,
                        f.header().height() - 1));
    if(!qtRect.isValid())    return;
    cv::Rect cvRect;
    cvhelp::qtRect2cvRect(qtRect, cvRect);
    cv::Mat crossReg(f.asCvMat(), cvRect);
    crossReg -= mean;
    cv::Mat crossReg_buf = crossReg;
//    crossReg_buf -= mean;
//    f.fillZeros();
    qDebug() << "before clone: " << crossReg.data;
    crossReg = crossReg_buf.clone();
    qDebug() << "after clone: " << crossReg.data;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
