#include "frameproc.h"
/////////////////////////////////////////////////////////////////////////////////////
void FrameProc::FrameProcSlot(void *pFrame,
                              int frameWidth,
                              int frameHeight)
{
    cv::Mat cvMat(frameHeight, frameWidth, CV_8UC1, pFrame);
    cv::blur(cvMat, cvMat, cv::Size(5,5));
    cv::Mat roi(cvMat, cv::Rect(100, 100, 300, 300));
    cv::namedWindow("roi");
    cv::imshow("roi", roi);
    emit DrawFrameSignal(pFrame, frameWidth, frameHeight);
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
