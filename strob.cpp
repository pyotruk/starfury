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
    //предобработка
    cv::Mat mainImg(frameHeight, frameWidth, CV_8UC1, pFrame);
    cv::blur(mainImg, mainImg, cv::Size(5,5));

    //создание, инициализация сигнального и фонового стробов
    int size2 = FSize / 2;
    QPoint topLeft(FCenter.x() - size2, FCenter.y() - size2);
    cv::Mat signalRoi(mainImg, cv::Rect(topLeft.x(),
                                        topLeft.y(),
                                        FSize,
                                        FSize));
    int foneStrobSize = (int)(floor(SQRT_2 * FSize + 0.5));
    cv::Mat foneRoi(mainImg, cv::Rect(topLeft.x(),
                                      topLeft.y(),
                                      foneStrobSize,
                                      foneStrobSize));

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
        cv::Rect trackingWindow(topLeft.x(),
                                topLeft.y(),
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
