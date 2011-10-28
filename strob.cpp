#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
Strob::Strob(QSettings * settings) :
    _settings(settings),
    _geometry(new StrobGeometry(_settings))
{    
    loadSettings(_settings);
}
/////////////////////////////////////////////////////////////////////////////////////
Strob::~Strob()
{
    saveSettings(_settings);
    delete _geometry;
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::loadSettings(QSettings *settings)
{
    if(settings == 0)
    {
        _threshold = _defaultThreshold;
    }
    else
    {
        _threshold = settings->value(SKEY_STROB_STDDEV_THRESHOLD, _defaultThreshold).toDouble();
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::saveSettings(QSettings *settings)
{
    if(settings != 0)
    {
        settings->setValue(SKEY_STROB_STDDEV_THRESHOLD, _threshold);
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::makeTracking(void *frame,
                         const int frameWidth,
                         const int frameHeight)
{
    _frameSize = QSize(frameWidth, frameHeight);
    cv::Mat mainImg(_frameSize.height(),
                    _frameSize.width(),
                    CV_8UC1,
                    frame);
    //создание, инициализация сигнального и фонового стробов
    _geometry->checkRange(_frameSize);
    cv::Rect innerRect;
    cv::Rect outerRect;
    qtRect2cvRect(_geometry->innerRect(), innerRect);
    qtRect2cvRect(_geometry->outerRect(), outerRect);
    cv::Mat signalRoi(mainImg, innerRect);
    cv::Mat foneRoi(mainImg, outerRect);
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
        cv::meanShift(mainImg, innerRect, crit);
        QRect newInnerRect;
        cvRect2qtRect(innerRect, newInnerRect);
        _geometry->setRect(newInnerRect);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::clickTarget(QMouseEvent *mousePressEvent)
{
    _geometry->setCenter(mousePressEvent->pos());
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
StrobGeometry &Strob::geometry()
{
    return *_geometry;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
