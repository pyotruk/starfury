#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
Strob::Strob(QSettings *settings,
             QThread *parent) :
    _settings(settings),
    _geometry(new StrobGeometry(_settings))
{    
    loadSettings(_settings);
    this->moveToThread(parent);
    _geometry->moveToThread(parent);
    qDebug() << "strob->_geometry thread:" << _geometry->thread();
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
void Strob::makeTracking(Frame *frame)
{
    //создание, инициализаци€ сигнального и фонового стробов
    _geometry->checkRange(QSize(frame->header().width, frame->header().height));
    cv::Rect innerRect;
    cv::Rect outerRect;
    qtRect2cvRect(_geometry->innerRect(), innerRect);
    qtRect2cvRect(_geometry->outerRect(), outerRect);
    cv::Mat signalRoi(frame->asCvMat(), innerRect);
    cv::Mat foneRoi(frame->asCvMat(), outerRect);

    cv::blur(foneRoi, foneRoi, cv::Size(5,5)); //фильтраци€

    double sumThreshold; /*порог по суммарным значени€м €ркости
                         в сигнальном и фоновом стробах*/
    int pixThreshold; //порог, приведЄнный к одному пикселу
    calcThresholds(signalRoi,
                   foneRoi,
                   _threshold, //порог в единицах — ќ
                   sumThreshold,
                   pixThreshold);

    if(sumThreshold > 0)    //проверка услови€ слежени€
    {
        cv::threshold(signalRoi, //порогова€ бинаризаци€ в сигнальном стробе
                      signalRoi,
                      pixThreshold,
                      0xFF * signalRoi.channels(),
                      cv::THRESH_TOZERO);

        //вычисление центра масс по сигнальному стробу
        cv::TermCriteria crit(cv::TermCriteria::COUNT, 1, 0.1);
        cv::meanShift(frame->asCvMat(), innerRect, crit);
        QRect newInnerRect;
        cvRect2qtRect(innerRect, newInnerRect);
        _geometry->setRect(newInnerRect);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::calcThresholds(const cv::Mat &signalRoi,
                           const cv::Mat &foneRoi,
                           const double stdDevThreshold,
                           double &sumThreshold,
                           int    &pixThreshold)
{
    double sumSignal = cv::sum(signalRoi)[0]; //сумма €ркости пикселей по сигнальному стробу
    double sumFone = cv::sum(foneRoi)[0]; //по фоновому стробу
    sumFone -= sumSignal; //на рамке
    double sumStdDev = sqrt(sumFone); //CKO
    sumThreshold = sumSignal - sumFone; /*порог по суммарным значени€м €ркости
                                          в сигнальном и фоновом стробах*/
    int borderPixNum = (foneRoi.total() - signalRoi.total()); //кол-во пикселей на рамке
    double fonePerPix = sumFone / borderPixNum;
    double stdDevPerPix = sumStdDev / borderPixNum;
    pixThreshold = (int)floor(0.5 + fonePerPix + stdDevThreshold * stdDevPerPix); //порог, приведЄнный к одному пикселу
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
