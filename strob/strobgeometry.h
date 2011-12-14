#ifndef STROBGEOMETRY_H
#define STROBGEOMETRY_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QSettings>
#include <math.h>
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
#define SKEY_STROBGEOMETRY_SIDE       "/Strob/Geometry/Side"
#define SKEY_STROBGEOMETRY_REFPOINT_X "/Strob/Geometry/RefPointX"
#define SKEY_STROBGEOMETRY_REFPOINT_Y "/Strob/Geometry/RefPointY"
/////////////////////////////////////////////////////////////////////////////////////
class StrobGeometry : public QObject
{
    Q_OBJECT
public:
    explicit StrobGeometry(QSettings *settings = 0);
    ~StrobGeometry();
    void setRect(const QRect &innerRect);
    int  innerSide();
    int  outerSide();
    QPoint &center();
    QPoint &refPoint();
    const QRect &innerRect();
    const QRect &outerRect();
    void checkRange(const QSize &imgSize);
public slots:
    void setSide(const int innerSide);
    void setCenter(const QPoint &center);
    void setRefPoint(const QPoint &refPoint);
private:
    static const double _sqrt2 = 1.4142135623730950488016887242097;
    static const int _defaultInnerSide = 40;
    static const int _defaultRefPointX = 0;
    static const int _defaultRefPointY = 0;
    QSettings *_settings;
    QRect     _innerRect;
    QRect     _outerRect;
    QPoint    _center;
    QPoint    _refPoint;
    void loadSettings(QSettings *settings);
    void saveSettings(QSettings *settings);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROBGEOMETRY_H
