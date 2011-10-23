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

typedef unsigned char uchar;

//setting keys
#define SKEY_STROB_SIZE       "/Strob/Size"
#define SKEY_STDDEV_THRESHOLD "/Strob/StdDevThreshold"


class Strob : public QObject
{
    Q_OBJECT
public:
    explicit Strob(int refPointX,
                   int refPointY,
                   QSettings * settings = 0);
    ~Strob();
    int    size();
    double threshold();
private:
    static const double SQRT_2 = 1.4142135623730950488016887242097;
    static const int    DEFAULT_SIZE = 40;
    static const double DEFAULT_STDDEV_THRESHOLD = 1.0;
    QSettings *FSettings;
    QPoint    FRefPoint;
    QPoint    FCenter;
    int       FSize;
    double    FStdDevThreshold;
    QSize     FFrameSize;
    void checkCenterRange(QPoint *center,
                          int frameWidth,
                          int frameHeight,
                          int roiSize);
    void loadSettings(QSettings *settings);
    void saveSettings(QSettings *settings);
private slots:
    void makeTracking(void *pFrame,
                      int frameWidth,
                      int frameHeight);
    void clickTarget(QMouseEvent *mousePressEvent);
    void setSize(int size);
    void setThreshold(int pos);
signals:
    void drawFrame(void *pFrame,
                   int frameWidth,
                   int frameHeight); //сигнал на отрисовку картинки
};

#endif // STROB_H
