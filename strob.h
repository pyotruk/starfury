#ifndef STROB_H
#define STROB_H
/////////////////////////////////////////////////////////////////////////////////////
#include <numeric>
#include <QObject>
#include <QPoint>
#include <math.h>
#include <QMouseEvent>
#include <QSize>
#include <QSettings>
#include "strobgeometry.h"
#include "cvhelpfun.h"
#include "frame.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
#define SKEY_STROB_STDDEV_THRESHOLD "/Strob/StdDevThreshold"
/////////////////////////////////////////////////////////////////////////////////////
class Strob : public QObject
{
    Q_OBJECT
public:
    explicit Strob(QSettings *settings = 0,
                   QThread *parent = 0);
    ~Strob();
    double threshold();
    StrobGeometry &geometry();
private:
    static const double _defaultThreshold = 1.0;
    QSettings     *_settings;
    StrobGeometry *_geometry;
    double        _threshold; //порог обнаружения в единицах СКО
    void calcThresholds(const cv::Mat &signalRoi,
                        const cv::Mat &foneRoi,
                        const double stdDevThreshold,
                        double &sumThreshold,
                        int    &pixThreshold);
    void loadSettings(QSettings *settings);
    void saveSettings(QSettings *settings);
public slots:
    void makeTracking(Frame *frame);
    void clickTarget(QMouseEvent *mousePressEvent);
    void setThreshold(const int pos);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROB_H
