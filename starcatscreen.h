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
#include "snudpsrv.h"
#include "starcatreader.h"
#include "astrocalc.h"
#include "skysegment.h"
#include "star.h"
#include "artifactbox.h"
#include "adapters.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
#define SKEY_SCREEN_WIDTH               "/Screen/Width"
#define SKEY_SCREEN_HEIGHT              "/Screen/Height"
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
    static const int _timeout = 20;
    static const int _defaultScreenWidth  = 640;
    static const int _defaultScreenHeight = 480;
    QSettings        *_settings;
    ArtifactBox      *_starBox;
    StarcatReader    *_starcatReader;
    SkySegment       *_segment;
    SnUdpSrv         *_snServer;
    QSize             _screen;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void proc(TelescopeVector&);
    void catStar2screenStar(TelescopeVector&,
                            Star&,
                            Artifact&);
private slots:
    void inputTelescopeVector(TelescopeVector*,
                              QReadWriteLock*);
signals:
    void catStarsReady(ArtifactBox*);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STARCATSCREEN_H