#ifndef STARCATSCREEN_H
#define STARCATSCREEN_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <QThread>
#include <QSettings>
#include <QSize>
#include <QSizeF>
#include <QtAlgorithms>
#include <QQueue>
/////////////////////////////////////////////////////////////////////////////////////
#include "common/globalskeys.h"
#include "com/snudpsrv.h"
#include "sky/starcatreader.h"
#include "math/astrocalc.h"
#include "sky/skysegment.h"
#include "boxes/star.h"
#include "boxes/artifact.h"
#include "utils/timeutils.h"
#include "boxes/telescope.h"
#include "common/logfile.h"
#include "sky/velocimeter.h"
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
class StarcatScreen : public QThread
{
    Q_OBJECT
public:
    explicit StarcatScreen(QSettings*,
                           LogFile*,
                           ArtifactBox*);
    ~StarcatScreen();
public slots:
    void setScreenSize(const int width,
                       const int height);
private:
    StarcatScreen(const StarcatScreen&) {}
    StarcatScreen& operator =(const StarcatScreen&) {return *this;}
    static const int _timeout = 20;
    static const int _defaultScreenWidth  = 640;
    static const int _defaultScreenHeight = 480;
    static const int _maxDelay = 100; //msec
    static const int _maxQueueSize = 500;
    QSettings           *_settings;
    LogFile             *_log;
    ArtifactBox         *_starBox;
    StarcatReader       *_starcatReader;
    SkySegment          *_segment;
    SnUdpSrv            *_snServer;
    QSize                _screen;
    Velocimeter          _velocimeter;
    ArtifactBox          _cache_Targets;
    TelescopeBox         _cache_TelescopePos;
    QQueue<TelescopeBox> _queue_TelescopePos;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void cookStars(const TelescopePos&,
                   const StarVector&,
                   ArtifactVector&,
                   SkySegment&);
    void catStar2screenStar(const TelescopePos&,
                            const Star&,
                            Artifact&);
    void screenStar2catStar(const TelescopePos&,
                            const Artifact&,
                            Star&);
    bool refreshQueueAndCheckDelay();
private slots:
    void inputTelescopePos(TelescopeBox*);
    void inputTargets(ArtifactBox*); //в экранной СК в плоскости каталога
signals:
    void catStarsReady(ArtifactBox*);
    void sendMeasureError(double errAlpha,  //rad
                          double errDelta); //rad
    void sendScreenVelocity(const double vx,
                            const double vy);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STARCATSCREEN_H
