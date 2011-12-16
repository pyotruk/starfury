#include "strobgeometry.h"
/////////////////////////////////////////////////////////////////////////////////////
StrobGeometry::StrobGeometry(QSettings *s) :
    _settings(s)
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
void StrobGeometry::loadSettings(QSettings *s)
{
    setSide(s->value(__skeyStrobSide, _defaultInnerSide).toInt());
    setRefPoint(QPoint(s->value(__skeyStrobRefPointX, _defaultRefPointX).toInt(),
                       s->value(__skeyStrobRefPointY, _defaultRefPointY).toInt()));
}
///////////////////////////////////////////////////////////////////////////////////////
void StrobGeometry::saveSettings(QSettings *s)
{
    s->setValue(__skeyStrobSide, _innerRect.width());
    s->setValue(__skeyStrobRefPointX, _refPoint.x());
    s->setValue(__skeyStrobRefPointY, _refPoint.y());
}
///////////////////////////////////////////////////////////////////////////////////////
void StrobGeometry::setSide(const int innerSide)
{
    int outerSide = (int)(floor(__sqrt2 * innerSide + 0.5));
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
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
