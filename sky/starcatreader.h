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
#include <QString>
/////////////////////////////////////////////////////////////////////////////////////
#include "common/globalskeys.h"
#include "sky/skysegment.h"
#include "boxes/star.h"
#include "utils/guidecodec.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
static const QString __skeyStarcatPath("/Starcat/Reader/Path");
static const QString __skeyStarcatMagnLim("/Starcat/Reader/MagnLim");
static const QString __skeyStarcatSegmentSide("/Starcat/Reader/Segment/Side");
static const QString __skeyStarcatSegmentEdge("/Starcat/Reader/Segment/Edge");
/////////////////////////////////////////////////////////////////////////////////////
static const QString __defaultStarcatPath("D:/GuideCat/Guide.rez");
/////////////////////////////////////////////////////////////////////////////////////
class StarcatReader : public QThread
{
public:
    explicit StarcatReader(QSettings*);
    ~StarcatReader();
    const StarVector* stars()   const {return _starVec1;}
    const SkySegment& segment() const {return _segment;}
    QMutex*           mutex()   const {return _mutex;}
    void refresh(const double alpha,
                 const double delta/*,
                 const double fieldWidth,
                 const double fieldHeight*/);
protected:
    void run();
private:
    StarcatReader(const StarcatReader&) {}
    StarcatReader& operator =(const StarcatReader&) {return *this;}
    static const double __pi = 3.1415926535897932384626433832795;
    static const double __deg2rad = 0.017453292519943295769236907684886;
    static const double __rad2deg = 57.295779513082320876798154814105;
    static const double _defaultMagnLim = 12.0;
    static const double _defaultFieldWidth  = 40; //ang min
    static const double _defaultFieldHeight = 30; //ang min
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