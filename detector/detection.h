#ifndef DETECTION_H
#define DETECTION_H
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#include "opencv.hpp"
#include "utils/cvhelpfun.h"
#include "math/astrocalc.h"
#include "boxes/frame.h"
#include "boxes/artifact.h"
/////////////////////////////////////////////////////////////////////////////////////
namespace detection
{
void filtering(Frame&);
void findArtifacts(Frame&,
                   ArtifactVector&,
                   const double magnThresh);
void deleteTarget(ArtifactVector&,
                  const QPointF &target);
bool isDoubleStar(const cv::Rect&);  //������������� �� cv::floodFill
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // DETECTION_H
