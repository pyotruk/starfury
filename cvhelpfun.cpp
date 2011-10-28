#include "cvhelpfun.h"
/////////////////////////////////////////////////////////////////////////////////////
void qtRect2cvRect(const QRect &qtRect,
                   cv::Rect &cvRect)
{
    cvRect = cv::Rect(qtRect.topLeft().x(),
                      qtRect.topLeft().y(),
                      qtRect.width(),
                      qtRect.height());
}
/////////////////////////////////////////////////////////////////////////////////////
void cvRect2qtRect(const cv::Rect &cvRect,
                   QRect &qtRect)
{
    qtRect = QRect(cvRect.x,
                   cvRect.y,
                   cvRect.width,
                   cvRect.height);
}
/////////////////////////////////////////////////////////////////////////////////////
