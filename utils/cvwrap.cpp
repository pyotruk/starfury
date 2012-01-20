#include "cvwrap.h"
/////////////////////////////////////////////////////////////////////////////////////
void cvwrap::qtRect2cvRect(const QRect &qtRect,
                           cv::Rect &cvRect)
{
    cvRect = cv::Rect(qtRect.topLeft().x(),
                      qtRect.topLeft().y(),
                      qtRect.width(),
                      qtRect.height());
}
/////////////////////////////////////////////////////////////////////////////////////
void cvwrap::cvRect2qtRect(const cv::Rect &cvRect,
                           QRect &qtRect)
{
    qtRect = QRect(cvRect.x,
                   cvRect.y,
                   cvRect.width,
                   cvRect.height);
}
/////////////////////////////////////////////////////////////////////////////////////
bool cvwrap::isBelongToCvRect(const int x,
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
void cvwrap::calcRectCenter(const cv::Rect &rect,
                            QPoint &center)
{
    center = QPoint(rect.tl().x + rect.width / 2,
                    rect.tl().y + rect.height / 2);
}
/////////////////////////////////////////////////////////////////////////////////////
int cvwrap::square(const cv::Rect &rect)
{
   return rect.width * rect.height;
}
/////////////////////////////////////////////////////////////////////////////////////
void cvwrap::otsuThreshold(cv::Mat &img)
{
    cv::threshold(img, img, 0, 0xFF, cv::THRESH_OTSU);
}
/////////////////////////////////////////////////////////////////////////////////////
void cvwrap::medianBlur(cv::Mat &f,
                        const int kernelSize)
{
    cv::medianBlur(f, f, kernelSize);
}
/////////////////////////////////////////////////////////////////////////////////////
void cvwrap::blur(cv::Mat &f,
                  const int kernelSize)
{
    cv::blur(f, f, cv::Size(kernelSize,
                            kernelSize));
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
