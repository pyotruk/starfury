#include "accumulator.h"
/////////////////////////////////////////////////////////////////////////////////////
const Frame& Accumulator::add(Frame &f)
{
    this->checkSize(f.header());
    this->checkNum();
    double mean = cv::mean(f.asCvMat())[0];
    cv::addWeighted(f.asCvMat(),
                    _alpha,
                    _frame.asCvMat(),
                    _beta,
                    0 - mean,
                    _frame.asCvMat());
    _frame.setTimeMarker(f.timeMarker());
    return _frame;
}
/////////////////////////////////////////////////////////////////////////////////////
void Accumulator::checkSize(const Frame::Header &newHeader)
{
    if(_frame.header() != newHeader)
    {
        _frame.setHeaderAndRealloc(newHeader);
        _frame.fillZeros();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Accumulator::checkNum()
{
    ++_num;
    if(_num > _capacity)
    {
        _num = 0;
        _frame.fillZeros();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
