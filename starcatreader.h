#ifndef STARCATREADER_H
#define STARCATREADER_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QMutex>
#include <QSettings>
#include <QString>
#include <QDebug>
#include <QTime>
#include <QtGlobal>
#include <QVector>
#include <qmath.h>
#include <QFile>
/////////////////////////////////////////////////////////////////////////////////////
#include "skysegment.h"
#include "star.h"
#include "guidecodec.h"
/////////////////////////////////////////////////////////////////////////////////////
#define pi                      3.1415926535897932384626433832795
#define DEG2RAD                 pi/180
#define RAD2DEG                 180/pi
#define DEFAULT_CATALOG_PATH    "D:/GuideCat/Guide.rez"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
#define SKEY_FIELD_WIDTH               "/Field/Width"
#define SKEY_FIELD_HEIGHT              "/Field/Height"
#define SKEY_STARCATREADER_PATH        "/Starcat/Reader/Path"
#define SKEY_STARCATREADER_MAGNLIM     "/Starcat/Reader/MagnLim"
#define SKEY_STARCATREADER_SEGMENTSIDE "/Starcat/Reader/Segment/Side"
#define SKEY_STARCATREADER_SEGMENTEDGE "/Starcat/Reader/Segment/Edge"
/////////////////////////////////////////////////////////////////////////////////////
class StarcatReader : public QThread
{
    Q_OBJECT
public:
    explicit StarcatReader(QSettings*);
    ~StarcatReader();
    StarVector* stars();
public slots:
    void refresh(double alpha, double delta);
private:
    static const double _defaultMagnLim = 15.0;
    static const double _defaultFieldWidth  = 0.025;
    static const double _defaultFieldHeight = 0.025;
    static const int    _defaultSegmentSide = 4;
    static const double _defaultSegmentEdge = 0.75;
    QSettings  *_settings;
    QString     _path;
    double      _magnLim;
    SkySegment  _segment;
    StarVector *_starVec0; //double buff
    StarVector *_starVec1;
    QFile       *_file;
    void readNewSegment();
    void switchBuffers();
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STARCATREADER_H
