#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
Strob::Strob(int refPointX,
             int refPointY,
             int fullSize,
             double stdDevThreshold)
    : FRefPoint(refPointX, refPointY),
      FCenter()
{
    FCenter = FRefPoint;
    FSize = fullSize;
    FStdDevThreshold = stdDevThreshold;
}
/////////////////////////////////////////////////////////////////////////////////////
Strob::~Strob()
{

}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::StrobSlot(void *pFrame,
                      int frameWidth,
                      int frameHeight)
{
    FFrameSize.setWidth(frameWidth);
    FFrameSize.setHeight(frameHeight);
    cv::Mat mainImg(frameHeight, frameWidth, CV_8UC1, pFrame);

    //создание, инициализация сигнального и фонового стробов
    int size2 = FSize / 2;
    int foneStrobSize = (int)(floor(SQRT_2 * FSize + 0.5));
    int foneStrobSize2 = foneStrobSize / 2;
    QPoint signalRoiTopLeft(FCenter.x() - size2,
                            FCenter.y() - size2);
    QPoint foneRoiTopLeft(FCenter.x() - foneStrobSize2,
                          FCenter.y() - foneStrobSize2);
    cv::Mat signalRoi(mainImg, cv::Rect(signalRoiTopLeft.x(),
                                        signalRoiTopLeft.y(),
                                        FSize,
                                        FSize));
    cv::Mat foneRoi(mainImg, cv::Rect(foneRoiTopLeft.x(),
                                      foneRoiTopLeft.y(),
                                      foneStrobSize,
                                      foneStrobSize));

    //предобработка
    cv::blur(foneRoi, foneRoi, cv::Size(5,5));

    //суммарная яркость в сигнальном стробе и в фоновом
    long int sumSignal = cv::sum(signalRoi)[0];
    long int sumFone = cv::sum(foneRoi)[0];
    sumFone -= sumSignal;

    //проверка условия слежения
    if(sumSignal > (sumFone + sqrt(sumFone)))
    {
        //пороговая бинаризация в сигнальном стробе
        int borderPixNum = (foneRoi.total() - signalRoi.total());
        double fonePerPix = sumFone / borderPixNum;
        double stdDev = sqrt(sumFone) / borderPixNum;
        int thresh = (int)floor(0.5 + fonePerPix + FStdDevThreshold * stdDev);
        cv::threshold(signalRoi,
                      signalRoi,
                      thresh,
                      0xFF * signalRoi.channels(),
                      cv::THRESH_TOZERO);

        //вычисление центра масс по сигнальному стробу
        cv::TermCriteria crit(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 2, 0.1);
        cv::Rect trackingWindow(signalRoiTopLeft.x(),
                                signalRoiTopLeft.y(),
                                FSize,
                                FSize);
        cv::meanShift(mainImg, trackingWindow, crit);
        FCenter.setX(trackingWindow.x + size2);
        FCenter.setY(trackingWindow.y + size2);
        CheckCenterRange(&FCenter,
                         frameWidth,
                         frameHeight,
                         FSize);
    }

    //посылка сигнала на отрисовку картинки
    emit DrawFrameSignal(pFrame, frameWidth, frameHeight);
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::CheckCenterRange(QPoint *center,
                             int frameWidth,
                             int frameHeight,
                             int roiSize)
{
    int k = 1;
    roiSize *= k;
    int xMin = 0 + roiSize;
    int xMax = frameWidth - roiSize;
    int yMin = 0 + roiSize;
    int yMax = frameHeight - roiSize;
    if(center->x() < xMin)  center->setX(xMin);
    if(center->x() > xMax)  center->setX(xMax);
    if(center->y() < yMin)  center->setY(yMin);
    if(center->y() > yMax)  center->setY(yMax);
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::ClickTarget(QMouseEvent *mousePressEvent)
{
    FCenter.setX(mousePressEvent->x());
    FCenter.setY(mousePressEvent->y());
    CheckCenterRange(&FCenter,
                     FFrameSize.width(),
                     FFrameSize.height(),
                     FSize);
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::StrobChange(int pos)
{
    FSize = pos;
}
/////////////////////////////////////////////////////////////////////////////////////
