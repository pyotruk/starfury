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
#include "utils/cvhelpfun.h"
#include "common/logfile.h"
#include "strob/strob_hf.h"
/////////////////////////////////////////////////////////////////////////////////////
/* setting keys */
static const QString __skeyStrobSide("/Strob/Geometry/Side");
static const QString __skeyStrobRefPointX("/Strob/Geometry/RefPointX");
static const QString __skeyStrobRefPointY("/Strob/Geometry/RefPointY");
static const QString __skeyStrobStddevThreshold("/Strob/StdDevThreshold");
/////////////////////////////////////////////////////////////////////////////////////
class Strob : public QObject
{
public:
    class Geometry
    {
    public:        
        static const int _defaultSide = 20;
        static const int _defaultRefPointX = 320;
        static const int _defaultRefPointY = 240;
        static const int _defaultFrameWidth  = 640;
        static const int _defaultFrameHeight = 480;
        static const double _defaultVelocityX = 0.0;
        static const double _defaultVelocityY = 0.0;
        static const int _minSide = 10;

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
        inline const QVector2D &velocity() const {return _velocity;}
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
        QRect     _clone_Signal;
        QRect     _clone_Fone;
        QRect     _signal;
        QRect     _fone;
        cv::Rect  _cache_Signal;
        cv::Rect  _cache_Fone;
        QPoint    _refPoint;
        QVector2D _velocity;
        QPoint    _cache_Center;
        bool      _rollback_ON;
        bool      _isValid;
        bool checkValid();
        void backup();
        void rollback();
        void refresh();
        void refreshFoneRect();
        inline void refreshValid() {_isValid = this->checkValid();}
    };

    enum RETURN_VALUES {OK = 0,
                        LOCKED = -1,        //strob is locked
                        BAD_GEOMETRY = -2}; //geometry out of range

    explicit Strob(QSettings*);
    ~Strob();
    inline double threshold()        const {return _threshold;}
    inline int    pixThreshold()     const {return _pixThreshold;}
    inline const Geometry& geometry() const {return _geometry;}
    inline double foneMean()   const {return _foneMean;}
    inline double signalMean() const {return _signalMean;}
    inline bool   isLocked()   const {return _locked;}
    void setThreshold(const double t) {_threshold = t;}
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
    Strob(const Strob&) {}
    Strob& operator =(const Strob&) {return *this;}
    static const double _defaultThreshold = 1.0;
    QSettings  *_settings;
    Geometry    _geometry;
    double      _threshold;       //регулируемый (руками) порог обнаружения в единицах СКО
    int         _pixThreshold;    //порог по яркости пиксела (используется для бинаризации cv::threshold)
    double      _foneMean;        //яркость в фоновом стробе на пиксель
    double      _signalMean;      //яркость в сигнальном стробе на пиксель
    bool        _locked;
    int         _timerId;
    void unlock();
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROB_H
