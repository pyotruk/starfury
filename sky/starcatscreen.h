#ifndef STARCATSCREEN_H
#define STARCATSCREEN_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QSettings>
#include <QDebug>
#include <QSize>
#include <QSizeF>
#include <QtAlgorithms>
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
    static const double __deg2rad = 0.017453292519943295769236907684886;
    static const double __rad2deg = 57.295779513082320876798154814105;
    static const int _timeout = 20;
    static const int _defaultScreenWidth  = 640;
    static const int _defaultScreenHeight = 480;
    static const int _maxTelescopeVectorSize = 20;
    QSettings        *_settings;
    LogFile          *_log;
    ArtifactBox      *_starBox;
    StarcatReader    *_starcatReader;
    SkySegment       *_segment;
    SnUdpSrv         *_snServer;
    QSize             _screen;
    TelescopeVector   _telescopeVec;
    Velocimeter       _velocimeter;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void cookStars(const TelescopeStatus&,
                   const StarVector&,
                   ArtifactVector&,
                   SkySegment&);
    void catStar2screenStar(const TelescopeStatus&,
                            const Star&,
                            Artifact&);
    void screenStar2catStar(const TelescopeStatus&,
                            const Artifact&,
                            Star&);
private slots:
    void inputTelescopePos(TelescopeBox*);
    void inputTarget(ArtifactBox*); //� �������� �� � ��������� ��������
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
