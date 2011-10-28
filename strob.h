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
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
#define SKEY_STROB_STDDEV_THRESHOLD "/Strob/StdDevThreshold"
/////////////////////////////////////////////////////////////////////////////////////
class Strob : public QObject
{
    Q_OBJECT
public:
    explicit Strob(QSettings *settings = 0);
    ~Strob();
    double threshold();
    StrobGeometry &geometry();
private:
    static const double _defaultThreshold = 1.0; //в единицах СКО
    QSettings     *_settings;
    StrobGeometry *_geometry;
    double        _threshold;
    QSize         _frameSize;
    void loadSettings(QSettings *settings);
    void saveSettings(QSettings *settings);
private slots:
    void makeTracking(void *frame,
                      const int frameWidth,
                      const int frameHeight);
    void clickTarget(QMouseEvent *mousePressEvent);
    void setThreshold(const int pos);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROB_H
