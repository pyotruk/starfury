#ifndef CVWRAP_H
#define CVWRAP_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QRect>
#include "opencv.hpp"
/////////////////////////////////////////////////////////////////////////////////////
namespace cvwrap
{
/* cv::Rect work */
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

/* img proc */
void otsuThreshold(cv::Mat&);
void medianBlur(cv::Mat &f,
                const int kernelSize);
void blur(cv::Mat&,
          const int kernelSize);
}
/////////////////////////////////////////////////////////////////////////////////////
#endif // CVWRAP_H
