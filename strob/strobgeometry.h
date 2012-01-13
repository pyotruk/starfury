#ifndef STROBGEOMETRY_H
#define STROBGEOMETRY_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QSize>
#include <QSettings>
#include <math.h>
#include <QString>
#include <QVector2D>
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
static const QString __skeyStrobSide("/Strob/Geometry/Side");
static const QString __skeyStrobRefPointX("/Strob/Geometry/RefPointX");
static const QString __skeyStrobRefPointY("/Strob/Geometry/RefPointY");
/////////////////////////////////////////////////////////////////////////////////////
class StrobGeometry
{
public:
    explicit StrobGeometry(QSettings*);
    ~StrobGeometry();
    void setRect(const QRect&);
    inline int side() const {return _signal.width();}
    const QPoint &center();
    inline const QPoint &refPoint() const {return _refPoint;}
    inline const QRect &signalRect() const {return _signal;}
    inline const QRect &foneRect()   const {return _fone;}
    inline int dx() const {return (_refPoint.x() - _signal.center().x());}
    inline int dy() const {return (_refPoint.y() - _signal.center().y());}
    void setSide(const int);
    void setCenter(const QPoint&);
    void setCenter(const int x,
                   const int y);
    void setRefPoint(const QPoint&);
    void setVelocity(const QPointF&);
    bool checkRange(const QSize &frameSize);
private:
    static const int _defaultInnerSide = 40;
    static const int _defaultRefPointX = 0;
    static const int _defaultRefPointY = 0;
    QSettings *_settings;
    QRect     _signal;
    QRect     _fone;
    QPoint    _cache_Center;
    QPoint    _refPoint;
    QVector2D _velocity;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void refreshFoneRect();
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROBGEOMETRY_H
