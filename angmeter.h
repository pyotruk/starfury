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
#include "artifactbox.h"
#include "astrocalc.h"
#include "triangle.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
#define SKEY_MAX_STAR_QUANTITY        "/Angmeter/MaxStarQuantity"
#define SKEY_EQUAL_EPS                "/Angmeter/EqualEps"
#define SKEY_SIMILAR_EPS              "/Angmeter/SimilarEps"
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
    static const int _defaultMaxStarQuantity = 15;
    static const double _defaultEqualEps   = 2.0;
    static const double _defaultSimilarEps = 7.0;
    QSettings    *_settings;
    ArtifactBox   _screenStars;
    ArtifactBox   _catStars;
    QSize         _screen;
    ArtifactBox   _equatedScreenStars;
    ArtifactBox   _equatedCatStars;
    int           _maxStarQuantity;
    double        _equalEps;
    double        _similarEps;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void proc(ArtifactVector &screenStars,
              ArtifactVector &catStars);
private slots:
    void inputScreenStars(ArtifactBox*);
    void inputCatStars(ArtifactBox*);
signals:
    void sendEquatedScreenStars(ArtifactBox*);
    void sendEquatedCatStars(ArtifactBox*);
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
