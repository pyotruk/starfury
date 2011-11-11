#ifndef STARCATSCREEN_H
#define STARCATSCREEN_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QMutex>
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
#include "artifact.h"
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
    explicit StarcatScreen(QSettings *settings);
    ~StarcatScreen();
public slots:
    void setScreenSize(const int width,
                       const int height);
private:
    static const int _timeout = 20;
    static const int _defaultScreenWidth  = 640;
    static const int _defaultScreenHeight = 480;
    QSettings       *_settings;
    TelescopeVector *_tvector;
    StarcatReader   *_starcatReader;
    ArtifactVector  *_stars;
    QMutex          *_mutex;
    SkySegment      *_segment;
    QSize           _screen;
    SnUdpSrv        *_snServer;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void processing();
    void catStar2screenStar(Star&, Artifact&);
private slots:
    void telescopeVectorIn(TelescopeVector*, QMutex*);
signals:
    void starsReady(ArtifactVector*, QMutex*);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STARCATSCREEN_H
