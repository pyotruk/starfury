#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
Strob::Strob(QSettings *s) :
    _settings(s),
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
void Strob::loadSettings(QSettings *s)
{
    _threshold = s->value(__skeyStrobStddevThreshold, _defaultThreshold).toDouble();
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::saveSettings(QSettings *s)
{
    s->setValue(__skeyStrobStddevThreshold, _threshold);
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::makeTracking(Frame &f)
{
    //создание, инициализаци€ сигнального и фонового стробов
    _geometry->checkRange(QSize(f.header().width(),
                                f.header().height()));
    cv::Rect innerRect;
    cv::Rect outerRect;
    cvhelp::qtRect2cvRect(_geometry->innerRect(), innerRect);
    cvhelp::qtRect2cvRect(_geometry->outerRect(), outerRect);
    cv::Mat signalRoi(f.asCvMat(), innerRect);
    cv::Mat foneRoi(f.asCvMat(), outerRect);
    cv::blur(foneRoi, foneRoi, cv::Size(5,5)); //фильтраци€

    double sumThreshold; /*порог по суммарным значени€м €ркости
                         в сигнальном и фоновом стробах*/
    calcThresholds(signalRoi,
                   foneRoi,
                   _threshold, //порог в единицах — ќ
                   sumThreshold,
                   _pixThreshold,
                   _signalMean,
                   _foneMean);

    if(sumThreshold > 0)    //проверка услови€ слежени€
    {
        cv::threshold(signalRoi, //порогова€ бинаризаци€ в сигнальном стробе
                      signalRoi,
                      _pixThreshold,
                      0xFF,
                      cv::THRESH_TOZERO);

        //вычисление центра масс по сигнальному стробу
        cv::TermCriteria crit(cv::TermCriteria::COUNT, 1, 0.1);
        cv::meanShift(f.asCvMat(), innerRect, crit);
        QRect newInnerRect;
        cvhelp::cvRect2qtRect(innerRect, newInnerRect);
        _geometry->setRect(newInnerRect);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::calcThresholds(const cv::Mat &signalRoi,
                           const cv::Mat &foneRoi,
                           const double stdDevThreshold,
                           double &sumThreshold,
                           int    &pixThreshold,
                           double &signalMean,
                           double &foneMean)
{
    double sumSignal = cv::sum(signalRoi)[0]; //сумма €ркости пикселей по сигнальному стробу
    signalMean = sumSignal / signalRoi.total();
    double sumFone = cv::sum(foneRoi)[0]; //по фоновому стробу
    sumFone -= sumSignal; //на рамке
    double sumStdDev = sqrt(sumFone); //CKO
    sumThreshold = sumSignal - sumFone; /*порог по суммарным значени€м €ркости
                                          в сигнальном и фоновом стробах*/
    int borderPixNum = (foneRoi.total() - signalRoi.total()); //кол-во пикселей на рамке
    foneMean = sumFone / borderPixNum;
    double stdDevPerPix = sumStdDev / borderPixNum;
    pixThreshold = (int)floor(0.5 + foneMean + stdDevThreshold * stdDevPerPix); //порог, приведЄнный к одному пикселу
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
