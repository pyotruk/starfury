#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
Strob::Strob(QSettings * settings,
             const int refPointX,
             const int refPointY) :
    _refPoint(refPointX, refPointY),
    _center(_refPoint),
    _settings(settings)
{    
    loadSettings(_settings);
}
/////////////////////////////////////////////////////////////////////////////////////
Strob::~Strob()
{
    saveSettings(_settings);
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::loadSettings(QSettings *settings)
{
    if(settings == 0)
    {
        _size = _defaultSize;
        _threshold = _defaultThreshold;
    }
    else
    {
        _size = settings->value(SKEY_STROB_SIZE, _defaultSize).toInt();
        _threshold = settings->value(SKEY_STDDEV_THRESHOLD, _defaultThreshold).toDouble();
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::saveSettings(QSettings *settings)
{
    if(settings != 0)
    {
        settings->setValue(SKEY_STROB_SIZE, _size);
        settings->setValue(SKEY_STDDEV_THRESHOLD, _threshold);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void Strob::makeTracking(void *frame,
                         const int frameWidth,
                         const int frameHeight)
{
    _frameSize.setWidth(frameWidth);
    _frameSize.setHeight(frameHeight);
    checkCenterRange(_center,
                     _frameSize.width(),
                     _frameSize.height(),
                     _size);
    cv::Mat mainImg(_frameSize.height(),
                    _frameSize.width(),
                    CV_8UC1,
                    frame);

    //создание, инициализация сигнального и фонового стробов
    int size2 = _size / 2;
    int foneStrobSize = (int)(floor(_sqrt2 * _size + 0.5));
    int foneStrobSize2 = foneStrobSize / 2;
    QPoint signalRoiTopLeft(_center.x() - size2,
                            _center.y() - size2);
    QPoint foneRoiTopLeft(_center.x() - foneStrobSize2,
                          _center.y() - foneStrobSize2);
    cv::Mat signalRoi(mainImg, cv::Rect(signalRoiTopLeft.x(),
                                        signalRoiTopLeft.y(),
                                        _size,
                                        _size));
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
        int thresh = (int)floor(0.5 + fonePerPix + _threshold * stdDev);
        cv::threshold(signalRoi,
                      signalRoi,
                      thresh,
                      0xFF * signalRoi.channels(),
                      cv::THRESH_TOZERO);

        //вычисление центра масс по сигнальному стробу
        cv::TermCriteria crit(cv::TermCriteria::COUNT, 1, 0.1);
        cv::Rect trackingWindow(signalRoiTopLeft.x(),
                                signalRoiTopLeft.y(),
                                _size,
                                _size);
        cv::meanShift(mainImg, trackingWindow, crit);
        _center.setX(trackingWindow.x + size2);
        _center.setY(trackingWindow.y + size2);
        checkCenterRange(_center,
                         _frameSize.width(),
                         _frameSize.height(),
                         _size);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::checkCenterRange(QPoint &center,
                             const int frameWidth,
                             const int frameHeight,
                             int roiSize)
{
    const double k = 0.75;
    roiSize *= k;
    int xMin = 0 + roiSize;
    int xMax = frameWidth - roiSize;
    int yMin = 0 + roiSize;
    int yMax = frameHeight - roiSize;
    if(center.x() < xMin)  center.setX(xMin);
    if(center.x() > xMax)  center.setX(xMax);
    if(center.y() < yMin)  center.setY(yMin);
    if(center.y() > yMax)  center.setY(yMax);
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::clickTarget(QMouseEvent *mousePressEvent)
{
    _center.setX(mousePressEvent->x());
    _center.setY(mousePressEvent->y());
    checkCenterRange(_center,
                     _frameSize.width(),
                     _frameSize.height(),
                     _size);
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::setSize(const int size)
{
    _size = size;
    checkCenterRange(_center,
                     _frameSize.width(),
                     _frameSize.height(),
                     _size);
}
/////////////////////////////////////////////////////////////////////////////////////
int Strob::size()
{
    return _size;
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::setThreshold(const int pos)
{
    _threshold = (double)pos;
}
/////////////////////////////////////////////////////////////////////////////////////
double Strob::threshold()
{
    return _threshold;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
