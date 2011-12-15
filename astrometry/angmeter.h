#ifndef ANGMETER_H
#define ANGMETER_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <qmath.h>
#include <QSize>
#include <QPoint>
/////////////////////////////////////////////////////////////////////////////////////
#include "globalskeys.h"
#include "math/astrocalc.h"
#include "astrometry/triangle.h"
#include "math/lincor.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
#define SKEY_MAX_STAR_QUANTITY        "/Angmeter/MaxStarQuantity"
#define SKEY_EQUATED_STAR_QUANTITY    "/Angmeter/EquatedStarQuantity"
#define SKEY_EQUAL_EPS                "/Angmeter/EqualEps"
#define SKEY_SIMILAR_EPS              "/Angmeter/SimilarEps"
#define SKEY_CHECK_EPS                "/Angmeter/CheckEps"
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
    ArtifactVector _picStars;
    ArtifactVector _catStars;
    TriangleBox    _tribox;
    int            _maxStarQuantity;
    int            _equatedStarQuantity;
    double         _equalEps;
    double         _similarEps;
    double         _checkEps;
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
    void sendTarget(double xTarget,
                    double yTarget);
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
