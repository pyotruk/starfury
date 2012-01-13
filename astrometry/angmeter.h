#ifndef ANGMETER_H
#define ANGMETER_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <qmath.h>
#include <QSize>
#include <QPointF>
#include <QString>
#include <QQueue>
/////////////////////////////////////////////////////////////////////////////////////
#include "common/globalskeys.h"
#include "math/astrocalc.h"
#include "math/lincor.h"
#include "common/logfile.h"
#include "astrometry/astrometry.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
static const QString __skeyMaxStarQuantity("/Angmeter/MaxStarQuantity");
static const QString __skeyMinEquatedStarQuantity("/Angmeter/MinEquatedStarQuantity");
static const QString __skeyEqualEps("/Angmeter/EqualEps");
static const QString __skeySimilarEps("/Angmeter/SimilarEps");
static const QString __skeyNearStarDist("/Angmeter/NearStarDist");
static const QString __skeyAstrometryMethod("/Angmeter/AstrometryMethod");
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
class Angmeter : public QThread
{
    Q_OBJECT
public:
    explicit Angmeter(QSettings*,
                      LogFile*,
                      ArtifactBox *picStars,
                      ArtifactBox *catStars,
                      ArtifactBox *targets);
    ~Angmeter();
    astrometry::METHOD method() const {return _method;}
public slots:
    void setScreenSize(const int width,
                       const int height);
    void setSimtriMethod()  {_method = astrometry::SIMTRI;}
    void setFreevecMethod() {_method = astrometry::FREEVEC;}
private:
    Angmeter(const Angmeter&) {}
    Angmeter& operator =(const Angmeter&) {return *this;}
    static const int _timeout = 20;
    static const int _defaultScreenWidth  = 640;
    static const int _defaultScreenHeight = 480;
    static const int _defaultMaxStarQuantity = 8;
    static const int _defaultMinEquatedStarQuantity = 4;
    static const double _defaultEqualEps     = 2.0;
    static const double _defaultSimilarEps   = 0.01;
    static const double _defaultNearStarDist = 20.0;
    static const int _maxDelay = 20; //msec
    static const astrometry::METHOD _defaultMethod = astrometry::SIMTRI;
    int                 _maxStarQuantity;
    int                 _minEquatedStarQuantity;
    double              _equalEps;
    double              _similarEps;
    double              _nearStarDist;
    astrometry::METHOD  _method;
    QSettings          *_settings;
    LogFile            *_log;
    ArtifactBox        *_picStars;
    ArtifactBox        *_catStars;
    ArtifactBox        *_targets;
    QSize               _screen;
    ArtifactBox         _cache_PicStars;
    ArtifactBox         _cache_CatStars;
    ArtifactBox         _cache_Targets;
    QQueue<ArtifactBox> _queue_CatStars;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void proc();
    void equation();
    bool checkEquation(const ArtifactVector &picStars,
                       const ArtifactVector &catStars,
                       const LinCor&,
                       const double equalEps);
    void correctTarget(const LinCor&,
                       Artifact&);
    void cookTarget(const LinCor&);
private slots:
    void inputTargets(ArtifactBox*);
    void inputScreenStars(ArtifactBox*);
    void inputCatStars(ArtifactBox*);
signals:
    void sendEquatedStars(ArtifactBox *pic,
                          ArtifactBox *cat);
    void sendTargets(ArtifactBox*);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // ANGMETER_H
