#ifndef STARCATSCREEN_H
#define STARCATSCREEN_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <QThread>
#include <QSettings>
#include <QSize>
#include <QSizeF>
#include <QtAlgorithms>
/////////////////////////////////////////////////////////////////////////////////////
#include "common/globalskeys.h"
#include "sky/starcatreader.h"
#include "math/astrocalc.h"
#include "astrometry/astrometry.h"
#include "sky/skysegment.h"
#include "boxes/star.h"
#include "boxes/artifact.h"
#include "utils/timeutils.h"
#include "com/telescope.h"
#include "sky/velocimeter.h"
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
class StarcatScreen : public QThread
{
    Q_OBJECT
public:
    explicit StarcatScreen(QSettings*,
                           ArtifactBox *shared_CatStars);
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
    static const int _maxDelay     = 100; //msec
    static const int _maxQueueSize = 500;

    /* shared */
    QSettings     *_settings;
    ArtifactBox   *_shared_CatStars;

    ArtifactBox    _cache_CatStars;
    TelBox         _cache_TelPos;
    StarcatReader *_catReader;
    SkySegment    *_segment;
    QSize          _screen;
    Velocimeter    _velocimeter;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void cookStars(const TelBox&,
                   const StarVector&,
                   ArtifactBox&);
private slots:
    void inputTelPos(TelBox*);
signals:
    void catStarsReady(ArtifactBox*);
    void sendScreenVelocity(const double vx,
                            const double vy);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STARCATSCREEN_H
