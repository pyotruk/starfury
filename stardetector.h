#ifndef STARDETECTOR_H
#define STARDETECTOR_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QMutex>
#include <QSettings>
#include <QtAlgorithms>
#include <QDebug>
#include <QDateTime>
#include <QPoint>
#include "opencv.hpp"
#include "cvhelpfun.h"
#include "frame.h"
#include "artifact.h"
#include "astrocalc.h"
#include "adapters.h"
/////////////////////////////////////////////////////////////////////////////////////
class StarDetector : public QThread
{
    Q_OBJECT
public:
    explicit StarDetector(QSettings *settings = 0);
    ~StarDetector();
private:
    static const int _timeout = 40;
    static const int _magnThresh = 2;
    QSettings      *_settings;
    Frame          *_frame;
    ArtifactVector *_artVec;
    QMutex         *_mutex;
    QPoint          _target;
    QDateTime       _time;
    void filtering(Frame*);
    void findArtifacts(Frame*,
                       ArtifactVector*,
                       double thresh);
    void deleteTarget(ArtifactVector&,
                      QPoint &target);
private slots:
    void frameIn(Frame*, QMutex*, int xTarget, int yTarget);
signals:
    void artifactsOut(ArtifactVector*, QMutex*, QDateTime*);
};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STARDETECTOR_H
