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
#include "com/telescope.h"
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
                      LogFile *starsLog,
                      LogFile *targetLog,
                      ArtifactBox *shared_eqPicStars,
                      ArtifactBox *shared_eqCatStars);
    ~Angmeter();
    inline astrometry::METHOD method() const {return _method;}
public slots:
    void setScreenSize(const int width,
                       const int height);
    void setFieldSize(const double width,
                      const double height);
    void setMethod(int m) {_method = (astrometry::METHOD)m;}
private:
    Angmeter(const Angmeter&) {}
    Angmeter& operator =(const Angmeter&) {return *this;}
    static const int _timeout = 20;
    static const int _defaultMaxStarQuantity = 8;
    static const int _defaultMinEquatedStarQuantity = 4;
    static const double _defaultEqualEps     = 2.0;
    static const double _defaultSimilarEps   = 0.01;
    static const double _defaultNearStarDist = 20.0;
    static const astrometry::METHOD _defaultMethod = astrometry::SIMTRI;
    static const int _maxDelay = 100; //msec
    static const int _maxQueueSize = 500;

    /* shared */
    QSettings          *_settings;
    LogFile            *_starsLog;
    LogFile            *_targetLog;
    ArtifactBox        *_shared_EqPicStars;
    ArtifactBox        *_shared_EqCatStars;

    int                 _maxStarQuantity;
    int                 _minEquatedStarQuantity;
    double              _equalEps;
    double              _similarEps;
    double              _nearStarDist;
    astrometry::METHOD  _method;
    QSizeF              _field;
    QSize               _screen;
    LinCor              _lincor;
    ArtifactBox         _cache_PicStars;
    QQueue<ArtifactBox> _queue_PicStars;
    ArtifactBox         _cache_CatStars;
    QQueue<ArtifactBox> _queue_CatStars;
    ArtifactBox         _cache_Targets;
    QQueue<ArtifactBox> _queue_Targets;
    TelBox              _cache_TelPos;
    QQueue<TelBox>      _queue_TelPos;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    bool procStars();
    void procTargets();
    void equation();
    bool checkEquation(const ArtifactVector &picStars,
                       const ArtifactVector &catStars,
                       const LinCor&,
                       const double equalEps);
    void correctTarget(const LinCor&,
                       Artifact&);
    void cookTarget(const LinCor&);
    bool cacheIsEmpty();
    bool queueIsEmpty();
    void refreshCache();
    bool checkQueueDelay();
private slots:
    void inputTargets(ArtifactBox*);
    void inputPicStars(ArtifactBox*);
    void inputCatStars(ArtifactBox*);
    void inputTelPos(TelBox*);
signals:
    void sendEquatedStars(ArtifactBox *pic,
                          ArtifactBox *cat);
    void sendMeasureError(double errAlpha,  //rad
                          double errDelta); //rad
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
