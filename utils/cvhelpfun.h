#ifndef CVHELPFUN_H
#define CVHELPFUN_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QRect>
#include "opencv.hpp"
/////////////////////////////////////////////////////////////////////////////////////
namespace cvhelp
{
void qtRect2cvRect(const QRect &qtRect,
                   cv::Rect &cvRect);

void cvRect2qtRect(const cv::Rect &cvRect,
                   QRect &qtRect);

bool isBelongToCvRect(const int x,
                      const int y,
                      const cv::Rect &rect);

void calcRectCenter(const cv::Rect &rect,
                    QPoint &center);
int square(const cv::Rect&);
}
/////////////////////////////////////////////////////////////////////////////////////
#endif // CVHELPFUN_H
