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
#include <QString>
/////////////////////////////////////////////////////////////////////////////////////
#include "strob/strobgeometry.h"
#include "utils/cvhelpfun.h"
#include "boxes/frame.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
static const QString __skeyStrobStddevThreshold("/Strob/StdDevThreshold");
/////////////////////////////////////////////////////////////////////////////////////
class Strob : public QThread
{
    Q_OBJECT
public:
    explicit Strob(QSettings*);
    ~Strob();
    double threshold()        const {return _threshold;}
    int    pixThreshold()     const {return _pixThreshold;}
    StrobGeometry &geometry() const {return *_geometry;}
    void makeTracking(Frame*);
public slots:
    void clickTarget(QMouseEvent *mousePressEvent);
    void setThreshold(const int pos);
private:
    Strob(const Strob&) {}
    Strob& operator =(const Strob&) {return *this;}
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
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROB_H
