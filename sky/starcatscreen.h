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
#include "globalskeys.h"
#include "com/snudpsrv.h"
#include "sky/starcatreader.h"
#include "math/astrocalc.h"
#include "sky/skysegment.h"
#include "boxes/star.h"
#include "boxes/artifact.h"
#include "utils/adapters.h"
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
class StarcatScreen : public QThread
{
    Q_OBJECT
public:
    explicit StarcatScreen(QSettings*,
                           ArtifactBox*);
    ~StarcatScreen();
public slots:
    void setScreenSize(const int width,
                       const int height);
private:
    static const double __deg2rad = 0.017453292519943295769236907684886;
    static const double __rad2deg = 57.295779513082320876798154814105;
    static const int _timeout = 20;
    static const int _defaultScreenWidth  = 640;
    static const int _defaultScreenHeight = 480;
    QSettings        *_settings;
    ArtifactBox      *_starBox;
    StarcatReader    *_starcatReader;
    SkySegment       *_segment;
    SnUdpSrv         *_snServer;
    QSize             _screen;
    TelescopeVector   _telescope;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void proc(const TelescopeVector&,
              StarVector&,
              ArtifactVector&,
              SkySegment&);
    void catStar2screenStar(const TelescopeVector&,
                            Star&,
                            Artifact&);
    void screenTarget2catTarget(const TelescopeVector&,
                                const double x,
                                const double y,
                                double &alpha,
                                double &delta);
private slots:
    void inputTelescopeVector(TelescopeVector*,
                              QReadWriteLock*);
    void inputTarget(double x, double y); //в экранной СК в плоскости каталога
signals:
    void catStarsReady(ArtifactBox*);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STARCATSCREEN_H
