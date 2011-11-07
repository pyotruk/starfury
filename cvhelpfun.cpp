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
bool isBelongToCvRect(const int x,
                      const int y,
                      const cv::Rect &rect)
{
    if((x > rect.tl().x) && (x < (rect.tl().x + rect.height)))
    {
        if((y > rect.tl().y) && (y < (rect.tl().y + rect.width)))
        {
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
void calcRectCenter(const cv::Rect &rect,
                    QPoint &center)
{
    center = QPoint(rect.tl().x + rect.width / 2,
                    rect.tl().y + rect.height / 2);
}
/////////////////////////////////////////////////////////////////////////////////////
