#ifndef STROB_H
#define STROB_H

#include "opencv.hpp"
#include <numeric>
#include <QObject>
#include <QPoint>
#include <math.h>
#include <QMouseEvent>
#include <QSize>

typedef unsigned char uchar;

class Strob : public QObject
{
    Q_OBJECT
public:
    Strob(int refPointX,
          int refPointY,
          int fullSize = DEFAULT_SIZE,
          double stdDevThreshold = DEFAULT_STDDEV_THRESHOLD);
    ~Strob();
private:
    static const double SQRT_2 = 1.4142135623730950488016887242097;
    static const int DEFAULT_SIZE = 40;
    static const double DEFAULT_STDDEV_THRESHOLD = 1.0;
    QPoint FRefPoint;
    QPoint FCenter;
    int    FSize;
    double FStdDevThreshold;
    QSize  FFrameSize;
    void CheckCenterRange(QPoint *center,
                          int frameWidth,
                          int frameHeight,
                          int roiSize);
private slots:
    void StrobSlot(void *pFrame,
                   int frameWidth,
                   int frameHeight);
    void ClickTarget(QMouseEvent *mousePressEvent);
    void StrobChange(int pos);
signals:
    void DrawFrameSignal(void *pFrame,
                         int frameWidth,
                         int frameHeight); //сигнал на отрисовку картинки
};

#endif // STROB_H
