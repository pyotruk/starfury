#include "strob_hf.h"
/////////////////////////////////////////////////////////////////////////////////////
/* help functions */
/////////////////////////////////////////////////////////////////////////////////////
void strob_hf::calcThresholdsAndRatings(const cv::Mat &signalRoi,
                                        const cv::Mat &foneRoi,
                                        const double stdDevThreshold,
                                        double &sumThreshold,
                                        int    &pixThreshold,
                                        double &signalRate,
                                        double &foneRate)
{
    double sumSignal = cv::sum(signalRoi)[0]; //сумма €ркости пикселей по сигнальному стробу
    signalRate = sumSignal / signalRoi.total();
    double sumFone = cv::sum(foneRoi)[0]; //сумма €ркости пикселей по фоновому стробу
    foneRate = sumFone / foneRoi.total();
    double sumStdDev = qSqrt(sumFone); //CKO
    double stdDevPerPix = sumStdDev / foneRoi.total();
    sumThreshold = sumSignal - sumFone; /*порог по суммарным значени€м €ркости
                                          в сигнальном и фоновом стробах*/
    pixThreshold = (int)qFloor(0.5 + foneRate + stdDevThreshold * stdDevPerPix); //порог, приведЄнный к одному пикселу
}
/////////////////////////////////////////////////////////////////////////////////////
int strob_hf::calcLockTime(const QVector2D velocity,
                           const int strobSide)
{
    if(velocity.isNull())
    {
        return 0;
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
