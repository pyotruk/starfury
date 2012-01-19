#include "strob_hf.h"
/////////////////////////////////////////////////////////////////////////////////////
/* help functions */
/////////////////////////////////////////////////////////////////////////////////////
void strob_hf::calcRatings(const cv::Mat &signalRoi,
                           const cv::Mat &foneRoi,
                           double &signalRate,
                           double &foneRate)
{
    signalRate = cv::mean(signalRoi)[0];
    foneRate = cv::mean(foneRoi)[0];
}
/////////////////////////////////////////////////////////////////////////////////////
int strob_hf::calcLockTime(const QVector2D velocity,
                           const int strobSide)
{
    if(velocity.isNull())
    {
        return -1;
    }
    const double lockSizeKoef = 2.0;
    double r = strobSide * _sqrt2 * lockSizeKoef;
    double dt = r / velocity.length() * 1000;
    return (int)dt;
}
/////////////////////////////////////////////////////////////////////////////////////
void strob_hf::calcCenterOfMass(cv::Mat     &frame,      //input
                                const QRect &signalRect, //input
                                QPoint &newCenter)  //output
{
    cv::Rect cvSignalRect;
    cvhelp::qtRect2cvRect(signalRect, cvSignalRect);
    cv::TermCriteria crit(cv::TermCriteria::COUNT, 1, 0.1);
    cv::meanShift(frame, cvSignalRect, crit);
    QRect newSignalRect;
    cvhelp::cvRect2qtRect(cvSignalRect, newSignalRect);
    newCenter = newSignalRect.center();
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
