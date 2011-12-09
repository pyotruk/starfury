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
#include "snudpsrv.h"
#include "starcatreader.h"
#include "astrocalc.h"
#include "skysegment.h"
#include "star.h"
#include "artifact.h"
#include "adapters.h"
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
    void proc(const TelescopeVector&,
              StarVector&,
              ArtifactVector&,
              SkySegment&);
    void catStar2screenStar(const TelescopeVector&,
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
