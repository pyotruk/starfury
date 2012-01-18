#ifndef STROB_HF_H
#define STROB_HF_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QSize>
#include <QVector2D>
#include <QRect>
#include <QPoint>
#include <qmath.h>
/////////////////////////////////////////////////////////////////////////////////////
#include "utils/cvhelpfun.h"
/////////////////////////////////////////////////////////////////////////////////////
namespace strob_hf //help functions
{
static const double _sqrt2 = 1.4142135623730950488016887242097;
void calcThresholdsAndRatings(const cv::Mat &signalRoi,
                              const cv::Mat &foneRoi,
                              const double stdDevThreshold, //порог в единицах — ќ
                              double &sumThreshold, //порог по суммарным значени€м €ркости
                              int    &pixThreshold, //порог по €ркости пиксела
                              double &signalRate,
                              double &foneRate);
int calcLockTime(const QVector2D velocity,
                 const int strobSide);
void calcCenterOfMass(cv::Mat     &frame,      //input
                      const QRect &signalRect, //input
                      QPoint &newCenter); //output
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROB_HF_H
