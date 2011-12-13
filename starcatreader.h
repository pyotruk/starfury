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
#include "globalskeys.h"
#include "skysegment.h"
#include "star.h"
#include "guidecodec.h"
/////////////////////////////////////////////////////////////////////////////////////
#define DEFAULT_CATALOG_PATH    "D:/GuideCat/Guide.rez"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
#define SKEY_STARCATREADER_PATH        "/Starcat/Reader/Path"
#define SKEY_STARCATREADER_MAGNLIM     "/Starcat/Reader/MagnLim"
#define SKEY_STARCATREADER_SEGMENTSIDE "/Starcat/Reader/Segment/Side"
#define SKEY_STARCATREADER_SEGMENTEDGE "/Starcat/Reader/Segment/Edge"
/////////////////////////////////////////////////////////////////////////////////////
class StarcatReader : public QThread
{
public:
    explicit StarcatReader(QSettings*);
    ~StarcatReader();
    StarVector* stars();
    SkySegment& segment();
    QMutex*     mutex();
    void refresh(const double alpha,
                 const double delta/*,
                 const double fieldWidth,
                 const double fieldHeight*/);
protected:
    void run();
private:
    static const double __pi = 3.1415926535897932384626433832795;
    static const double __deg2rad = 0.017453292519943295769236907684886;
    static const double __rad2deg = 57.295779513082320876798154814105;
    static const double _defaultMagnLim = 15.0;
    static const double _defaultFieldWidth  = 0.025;
    static const double _defaultFieldHeight = 0.025;
    static const int    _defaultSegmentSide = 4;
    static const double _defaultSegmentEdge = 0.75;
    static const unsigned long _termTimeout = 1000;
    static const int           _timeout = 20;
    QSettings   *_settings;
    QString      _path;
    double       _magnLim;
    SkySegment   _segment;
    StarVector  *_starVec0; //double buff
    StarVector  *_starVec1;
    QFile       *_file;
    QMutex      *_mutex;
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
