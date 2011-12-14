#ifndef STROB_H
#define STROB_H
/////////////////////////////////////////////////////////////////////////////////////
#include <numeric>
#include <QThread>
#include <QPoint>
#include <math.h>
#include <QMouseEvent>
#include <QSize>
#include <QSettings>
/////////////////////////////////////////////////////////////////////////////////////
#include "strob/strobgeometry.h"
#include "utils/cvhelpfun.h"
#include "boxes/frame.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
#define SKEY_STROB_STDDEV_THRESHOLD "/Strob/StdDevThreshold"
/////////////////////////////////////////////////////////////////////////////////////
class Strob : public QThread
{
    Q_OBJECT
public:
    explicit Strob(QSettings*);
    ~Strob();
    double threshold();
    int    pixThreshold();
    StrobGeometry &geometry();
    void makeTracking(Frame*);
private:
    static const double _defaultThreshold = 1.0;
    QSettings     *_settings;
    StrobGeometry *_geometry;
    double        _threshold; //порог обнаружения в единицах СКО
    int           _pixThreshold; //порог по яркости пиксела (используется для бинаризации cv::threshold)
    void calcThresholds(const cv::Mat &signalRoi,
                        const cv::Mat &foneRoi,
                        const double stdDevThreshold,
                        double &sumThreshold,
                        int    &pixThreshold);
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
public slots:
    void clickTarget(QMouseEvent *mousePressEvent);
    void setThreshold(const int pos);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROB_H
