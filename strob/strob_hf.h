#ifndef STROB_HF_H
#define STROB_HF_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QSize>
#include <QVector2D>
#include <QRect>
#include <QPoint>
#include <qmath.h>
/////////////////////////////////////////////////////////////////////////////////////
#include "utils/cvwrap.h"
/////////////////////////////////////////////////////////////////////////////////////
namespace strob_hf //help functions
{
static const double _sqrt2 = 1.4142135623730950488016887242097;
void calcRatings(const cv::Mat &signalRoi,
                 const cv::Mat &foneRoi,
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
