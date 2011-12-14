#include "strobgeometry.h"
/////////////////////////////////////////////////////////////////////////////////////
StrobGeometry::StrobGeometry(QSettings *settings) :
    _settings(settings)
{
    loadSettings(_settings);
    setCenter(_refPoint);
}
/////////////////////////////////////////////////////////////////////////////////////
StrobGeometry::~StrobGeometry()
{
    saveSettings(_settings);
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobGeometry::loadSettings(QSettings *settings)
{
    if(settings == 0)
    {
        setSide(_defaultInnerSide);
        setRefPoint(QPoint(_defaultRefPointX, _defaultRefPointY));
    }
    else
    {
        setSide(settings->value(SKEY_STROBGEOMETRY_SIDE, _defaultInnerSide).toInt());
        setRefPoint(QPoint(settings->value(SKEY_STROBGEOMETRY_REFPOINT_X, _defaultRefPointX).toInt(),
                           settings->value(SKEY_STROBGEOMETRY_REFPOINT_Y, _defaultRefPointY).toInt()));
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void StrobGeometry::saveSettings(QSettings *settings)
{
    if(settings != 0)
    {
        settings->setValue(SKEY_STROBGEOMETRY_SIDE, _innerRect.width());
        settings->setValue(SKEY_STROBGEOMETRY_REFPOINT_X, _refPoint.x());
        settings->setValue(SKEY_STROBGEOMETRY_REFPOINT_Y, _refPoint.y());
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void StrobGeometry::setSide(const int innerSide)
{
    int outerSide = (int)(floor(_sqrt2 * innerSide + 0.5));
    _innerRect.setSize(QSize(innerSide, innerSide));
    _outerRect.setSize(QSize(outerSide, outerSide));
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobGeometry::setCenter(const QPoint &center)
{
    _center = center;
    _innerRect.moveCenter(_center);
    _outerRect.moveCenter(_center);
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobGeometry::setRefPoint(const QPoint &refPoint)
{
    _refPoint = refPoint;
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobGeometry::setRect(const QRect &innerRect)
{
    QPoint cen(innerRect.x() + innerRect.width() / 2,
               innerRect.y() + innerRect.height() / 2);
    setCenter(cen);
}
/////////////////////////////////////////////////////////////////////////////////////
int StrobGeometry::innerSide()
{
    return _innerRect.width();
}
/////////////////////////////////////////////////////////////////////////////////////
int StrobGeometry::outerSide()
{
    return _outerRect.width();
}
/////////////////////////////////////////////////////////////////////////////////////
QPoint &StrobGeometry::center()
{
    return _center;
}
/////////////////////////////////////////////////////////////////////////////////////
QPoint &StrobGeometry::refPoint()
{
    return _refPoint;
}
/////////////////////////////////////////////////////////////////////////////////////
const QRect &StrobGeometry::innerRect()
{
    return _innerRect;
}
/////////////////////////////////////////////////////////////////////////////////////
const QRect &StrobGeometry::outerRect()
{
    return _outerRect;
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobGeometry::checkRange(const QSize &imgSize)
{
    QPoint cen = _center;
    int side2 = ceil(_outerRect.width() / 2 + 1);
    int xMin = 0 + side2;
    int xMax = imgSize.width() - side2;
    int yMin = 0 + side2;
    int yMax = imgSize.height() - side2;
    if(cen.x() < xMin)  cen.setX(xMin);
    if(cen.x() > xMax)  cen.setX(xMax);
    if(cen.y() < yMin)  cen.setY(yMin);
    if(cen.y() > yMax)  cen.setY(yMax);
    setCenter(cen);
}
/////////////////////////////////////////////////////////////////////////////////////
