#ifndef STROB_H
#define STROB_H

#include "opencv.hpp"
#include <numeric>
#include <QObject>
#include <QPoint>
#include <math.h>
#include <QMouseEvent>
#include <QSize>
#include <QSettings>

//setting keys
#define SKEY_STROB_SIZE       "/Strob/Size"
#define SKEY_STDDEV_THRESHOLD "/Strob/StdDevThreshold"


class Strob : public QObject
{
    Q_OBJECT
public:
    explicit Strob(QSettings *settings = 0,
                   const int refPointX = 0,
                   const int refPointY = 0);
    ~Strob();
    int    size();
    double threshold();
private:
    static const double _sqrt2 = 1.4142135623730950488016887242097;
    static const int    _defaultSize = 40;
    static const double _defaultThreshold = 1.0; //в единицах СКО
    QPoint    _refPoint;
    QPoint    _center;
    QSettings *_settings;
    int       _size;
    double    _threshold;
    QSize     _frameSize;
    void checkCenterRange(QPoint &center,
                          const int frameWidth,
                          const int frameHeight,
                          int roiSize);
    void loadSettings(QSettings *settings);
    void saveSettings(QSettings *settings);
private slots:
    void makeTracking(void *frame,
                      const int frameWidth,
                      const int frameHeight);
    void clickTarget(QMouseEvent *mousePressEvent);
    void setSize(const int size);
    void setThreshold(const int pos);
};

#endif // STROB_H
