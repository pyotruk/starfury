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
/////////////////////////////////////////////////////////////////////////////////////
#include "common/globalskeys.h"
#include "math/astrocalc.h"
#include "math/lincor.h"
#include "astrometry/identifier.h"
#include "common/logfile.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
static const QString __skeyMaxStarQuantity("/Angmeter/MaxStarQuantity");
static const QString __skeyMinEquatedStarQuantity("/Angmeter/MinEquatedStarQuantity");
static const QString __skeyEqualEps("/Angmeter/EqualEps");
static const QString __skeySimilarEps("/Angmeter/SimilarEps");
static const QString __skeyNearStarDist("/Angmeter/NearStarDist");
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
class Angmeter : public QThread
{
    Q_OBJECT
public:
    explicit Angmeter(QSettings*,
                      LogFile*,
                      ArtifactBox *equatedPicStars,
                      ArtifactBox *equatedCatStars,
                      TargetBox   *target);
    ~Angmeter();
public slots:
    void setScreenSize(const int width,
                       const int height);
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
    static const double _defaultNearStarDist = 25.0;
    static const int _maxDelay = 10; //msec
    int            _maxStarQuantity;
    int            _minEquatedStarQuantity;
    double         _equalEps;
    double         _similarEps;
    double         _nearStarDist;
    QSettings     *_settings;
    LogFile       *_log;
    ArtifactBox   *_equatedPicStars;
    ArtifactBox   *_equatedCatStars;
    TargetBox     *_target;
    QSize          _screen;
    ArtifactBox    _rawPicStars;
    ArtifactBox    _rawCatStars;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void proc(const QPointF &target);
    void equation();
    bool checkEquation(const ArtifactVector &picStars,
                       const ArtifactVector &catStars,
                       const LinCor&,
                       const double equalEps);
    void correctTarget(const LinCor&,
                       Artifact&);
    bool checkTimeDelay(const ArtifactBox &a1,
                        const ArtifactBox &a2,
                        const int maxDelay);
    void cookTarget(const QPointF   &target,
                    const QDateTime &timeMarker,
                    const LinCor    &cor,
                    TargetBox       &targetBox);
private slots:
    void inputScreenStars(ArtifactBox*,
                          double xTarget,
                          double yTarget);
    void inputCatStars(ArtifactBox*);
signals:
    void sendEquatedStars(ArtifactBox *pic,
                          ArtifactBox *cat);
    void sendTarget(TargetBox*);
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
