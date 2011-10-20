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
};

#endif // FRAMEPROC_H
