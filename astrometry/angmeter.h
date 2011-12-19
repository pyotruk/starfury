#ifndef ANGMETER_H
#define ANGMETER_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <qmath.h>
#include <QSize>
#include <QPoint>
#include <QString>
/////////////////////////////////////////////////////////////////////////////////////
#include "globalskeys.h"
#include "math/astrocalc.h"
#include "astrometry/triangle.h"
#include "math/lincor.h"
#include "astrometry/identifier.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
static const QString __skeyMaxStarQuantity("/Angmeter/MaxStarQuantity");
static const QString __skeyEquatedStarQuantity("/Angmeter/EquatedStarQuantity");
static const QString __skeyEqualEps("/Angmeter/EqualEps");
static const QString __skeySimilarEps("/Angmeter/SimilarEps");
static const QString __skeyCheckEps("/Angmeter/CheckEps");
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
class Angmeter : public QThread
{
    Q_OBJECT
public:
    explicit Angmeter(QSettings*);
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
    static const int _defaultMaxStarQuantity     = 8;
    static const int _defaultEquatedStarQuantity = 4;
    static const double _defaultEqualEps   = 3.0;
    static const double _defaultSimilarEps = 0.005;
    static const double _defaultCheckEps   = 2.0;
    QSettings     *_settings;
    QSize          _screen;
    ArtifactBox    _picStars;
    ArtifactBox    _catStars;
    TriangleBox    _tribox;
    int            _maxStarQuantity;
    int            _equatedStarQuantity;
    double         _equalEps;
    double         _similarEps;
    double         _checkEps;
    TargetBox      _target;
    ArtifactBox    _eqPicStars; //equated
    ArtifactBox    _eqCatStars; //equated
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void equation();
    bool checkEquation(const ArtifactVector &picStars,
                       const ArtifactVector &catStars,
                       const LinCor&,
                       const double eps);
    void correctTarget(const LinCor&,
                       Artifact&);
private slots:
    void inputScreenStars(ArtifactBox*,
                          double xTarget,
                          double yTarget);
    void inputCatStars(ArtifactBox*);
signals:
    void sendTriangles(TriangleBox*);
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
