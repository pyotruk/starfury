#ifndef STROB_H
#define STROB_H
/////////////////////////////////////////////////////////////////////////////////////
#include <numeric>
#include <QPoint>
#include <qmath.h>
#include <QSize>
#include <QSettings>
#include <QString>
#include <QObject>
#include <QTimerEvent>
/////////////////////////////////////////////////////////////////////////////////////
#include "strob/strobgeometry.h"
#include "utils/cvhelpfun.h"
#include "boxes/frame.h"
#include "common/logfile.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
static const QString __skeyStrobStddevThreshold("/Strob/StdDevThreshold");
/////////////////////////////////////////////////////////////////////////////////////
class Strob : public QObject
{
public:
    explicit Strob(QSettings*);
    ~Strob();
    double threshold()        const {return _threshold;}
    int    pixThreshold()     const {return _pixThreshold;}
    StrobGeometry &geometry() const {return *_geometry;}
    void makeTracking(Frame&);
    void setThreshold(const double t) {_threshold = t;}
    double foneMean()   const {return _foneMean;}
    double signalMean() const {return _signalMean;}
protected:
    void timerEvent(QTimerEvent *);
private:
    Strob(const Strob&) {}
    Strob& operator =(const Strob&) {return *this;}
    static const double _sqrt2 = 1.4142135623730950488016887242097;
    static const double _defaultThreshold = 1.0;
    static const double _lockSizeKoef = 2.0;
    QSettings     *_settings;
    StrobGeometry *_geometry;
    double        _threshold;       //регулируемый (руками) порог обнаружения в единицах СКО
    int           _pixThreshold;    //порог по яркости пиксела (используется для бинаризации cv::threshold)
    double        _foneMean;        //яркость в фоновом стробе на пиксель
    double        _signalMean;      //яркость в сигнальном стробе на пиксель
    bool          _locked;
    int           _timerId;
    void calcThresholds(const cv::Mat &signalRoi,
                        const cv::Mat &foneRoi,
                        const double stdDevThreshold,
                        double &sumThreshold,
                        int    &pixThreshold,
                        double &signalMean,
                        double &foneMean);
    int lockTime() const;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROB_H
