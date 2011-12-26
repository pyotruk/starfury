#ifndef STROBGEOMETRY_H
#define STROBGEOMETRY_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QSettings>
#include <math.h>
#include <QString>
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
static const QString __skeyStrobSide("/Strob/Geometry/Side");
static const QString __skeyStrobRefPointX("/Strob/Geometry/RefPointX");
static const QString __skeyStrobRefPointY("/Strob/Geometry/RefPointY");
/////////////////////////////////////////////////////////////////////////////////////
class StrobGeometry : public QObject
{
    Q_OBJECT
public:
    explicit StrobGeometry(QSettings*);
    ~StrobGeometry();
    void setRect(const QRect &innerRect);
    int  innerSide() const {return _innerRect.width();}
    int  outerSide() const {return _outerRect.width();}
    const QPoint &center()   const {return _center;}
    const QPoint &refPoint() const {return _refPoint;}
    const QRect &innerRect() const {return _innerRect;}
    const QRect &outerRect() const {return _outerRect;}
    void checkRange(const QSize &imgSize);
    void getError(int &dx, int &dy) const;
public slots:
    void setSide(const int innerSide);
    void setCenter(const QPoint &center);
    void setRefPoint(const QPoint &refPoint);
private:
    static const double __sqrt2 = 1.4142135623730950488016887242097;
    static const int _defaultInnerSide = 40;
    static const int _defaultRefPointX = 0;
    static const int _defaultRefPointY = 0;
    QSettings *_settings;
    QRect     _innerRect;
    QRect     _outerRect;
    QPoint    _center;
    QPoint    _refPoint;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROBGEOMETRY_H
