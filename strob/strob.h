#ifndef STROB_H
#define STROB_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <qmath.h>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QSize>
#include <QVector2D>
#include <QSettings>
#include <QString>
#include <QObject>
#include <QTimerEvent>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/frame.h"
#include "boxes/artifact.h"
#include "utils/cvwrap.h"
#include "common/logfile.h"
#include "strob/strob_hf.h"
/////////////////////////////////////////////////////////////////////////////////////
/* setting keys */
static const QString __skeyStrobSide("/Strob/Geometry/Side");
static const QString __skeyStrobRefPointX("/Strob/Geometry/RefPointX");
static const QString __skeyStrobRefPointY("/Strob/Geometry/RefPointY");
/////////////////////////////////////////////////////////////////////////////////////
class Strob : public QObject
{
public:
    class Geometry
    {
    public:        
        static const int _defaultSide = 20;
        static const int _minSide = 10;
        static const int _maxSide = 100;
        static const int _defaultRefPointX = 320;
        static const int _defaultRefPointY = 240;
        static const int _defaultFrameWidth  = 640;
        static const int _defaultFrameHeight = 480;
        static const int _minFrameWidth  = 400;
        static const int _minFrameHeight = 300;
        static const double _defaultVelocityX = 0.0;
        static const double _defaultVelocityY = 0.0;
        static const double _foneRectSideKoef = 1.5;

        explicit Geometry();
        const QPoint &center();
        inline int side() const {return _signal.width();}
        inline const QPoint &refPoint() const {return _refPoint;}
        inline const QRect &signalRect() const {return _signal;}
        inline const QRect &foneRect()   const {return _fone;}
        const cv::Rect& signalCvRect();
        const cv::Rect& foneCvRect();
        inline int dx() const {return (_refPoint.x() - _signal.center().x());}
        inline int dy() const {return (_refPoint.y() - _signal.center().y());}
        inline const QSize&     frameSize() const {return _frameSize;}
        void setSide(const int);
        void setCenter(const QPoint&);
        void setRefPoint(const QPoint&);
        void setVelocity(const QPointF&);
        void setFrameSize(const QSize&);
        void moveToRefPoint();
        void moveToFrameCenter();
        bool isValid() const {return _isValid;}

    private:        
        QSize     _frameSize;
        QRect     _backup_Signal;
        QRect     _backup_Fone;
        QRect     _signal;
        QRect     _fone;
        cv::Rect  _cache_Signal;
        cv::Rect  _cache_Fone;
        QPoint    _refPoint;
        QVector2D _velocity;        //нормализованна€ скорость в экранной —  [pix/sec]
        QPoint    _cache_Center;
        bool      _isValid;
        bool      _autoRefresh;
        void verifSide(int&);
        void verifFrameSize(QSize&);
        bool checkValid();
        void backup();
        void rollback();
        void refresh();
        void refreshFoneRect();
        void refreshValid();
    };

    enum RETURN_VALUES {OK = 0,
                        LOCKED = -1,        //strob is locked
                        BAD_GEOMETRY = -2}; //geometry out of range

    explicit Strob(QSettings*);
    ~Strob();
    inline const Geometry& geometry() const {return _geometry;}
    inline double foneRate()   const {return _foneRate;}
    inline double signalRate() const {return _signalRate;}
    inline bool   isLocked()   const {return _locked;}
    void setSide(const int);
    void setCenter(const QPoint&);
    void setCenter(const QPointF&);
    void setRefPoint(const QPoint&);
    void setVelocity(const QPointF&);
    void toArtifact(Artifact&);
    void moveToRefPoint();
    RETURN_VALUES proc(Frame&);

protected:
    void timerEvent(QTimerEvent *);

private:
    static const int _blurKernelSize = 5;
    Strob(const Strob&) {}
    Strob& operator =(const Strob&) {return *this;}
    QSettings  *_settings;
    Geometry    _geometry;
    double      _foneRate;        //€ркость в фоновом стробе на пиксель
    double      _signalRate;      //€ркость в сигнальном стробе на пиксель
    bool        _locked;
    int         _timerId;
    QVector2D   _velocity;        //скорость в экранной —  [pix/sec]
    void unlock();
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROB_H
