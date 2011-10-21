#ifndef FRAMEPROC_H
#define FRAMEPROC_H

#include "opencv.hpp"
#include <QObject>

typedef unsigned char uchar;

class FrameProc : public QObject
{
    Q_OBJECT
    private slots:
        void FrameProcSlot(void *pFrame,
                           int frameWidth,
                           int frameHeight);
    signals:
        void DrawFrameSignal(void *pFrame,
                             int frameWidth,
                             int frameHeight); //сигнал на отрисовку картинки
};

#endif // FRAMEPROC_H
