#include "astrocalc.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    //UTILS
////////////////////////////////////////////////////////////////////////////////
double ac::frac(const double x)
{
    return (x - qFloor(x));
}
////////////////////////////////////////////////////////////////////////////////
int ac::round(const double x)
{
    return qFloor(0.5 + x);
}
////////////////////////////////////////////////////////////////////////////////
double ac::calcDistance(const QPointF &p1,
                        const QPointF &p2)
{
    return qSqrt( qPow((p1.x() - p2.x()), 2) +
                  qPow((p1.y() - p2.y()), 2) );
}
////////////////////////////////////////////////////////////////////////////////
bool ac::isEqual(const QPointF &p1,
                 const QPointF &p2,
                 const double eps)
{
    double dist = calcDistance(p1, p2);
    if(dist < eps)      return true;
    else                return false;
}
////////////////////////////////////////////////////////////////////////////////
double shiftMinus(const double angle)
/* 0..360  -->  -180..+180 */
{
    if(angle > __pi)
        return (angle - __2pi);
    else
        return angle;
}
////////////////////////////////////////////////////////////////////////////////
double shiftPlus(const double angle)
/* -180..+180  -->  0..360 */
{
    if(angle < 0)
        return (__2pi + angle);
    else
        return angle;
}
////////////////////////////////////////////////////////////////////////////////
double astro2math(const double azimuth)
/* 0..360 левая СК (астрономический азимут) --->  -180..+180 правая СК */
{
  return (0 - shiftMinus(azimuth));
}
////////////////////////////////////////////////////////////////////////////////
double math2astro(const double azimuth)
/* -180..+180 правая СК  --->  0..360 левая СК (астрономический азимут) */
{
  return shiftPlus(0 - azimuth);
}
////////////////////////////////////////////////////////////////////////////////
void cartesian2spherical(const double x,
                         const double y,
                         const double z,
                         double &azimuth,
                         double &elevation)
{
    azimuth   = qAtan2(y, x);
    elevation = qAtan2(z, qSqrt(x*x + y*y));
}
////////////////////////////////////////////////////////////////////////////////
void spherical2cartesian(const double azimuth,
                         const double elevation,
                         double &x,
                         double &y,
                         double &z)
{
    x = qCos( azimuth ) * qCos( elevation );
    y = qSin( azimuth ) * qCos( elevation );
    z = qSin( elevation );
}
////////////////////////////////////////////////////////////////////////////////
void ac::rad2degminsec(const double rad,
                   int &deg,
                   int &min,
                   int &sec)
{
    double degAng = rad * __rad2deg;
    deg = qFloor(degAng);
    double foo = frac(degAng) * 60;
    min = qFloor(foo);
    sec = qFloor(frac(foo) * 60);
}
////////////////////////////////////////////////////////////////////////////////
void ac::rad2hourminsec(const double rad,
                    int &hour,
                    int &min,
                    int &sec)
{
    double hourAng = rad * __rad2hour;
    hour = qFloor(hourAng);
    double foo = frac(hourAng) * 60;
    min = qFloor(foo);
    sec = qFloor(frac(foo) * 60);
}
////////////////////////////////////////////////////////////////////////////////
void ac::hourminsec2degminsec(const int hour,
                          const int hourMin,
                          const int hourSec,
                          int &deg,
                          int &degMin,
                          int &degSec)
{
    double rad = (hour + hourMin / 60 + hourSec / 3600) * __hour2rad;
    rad2degminsec(rad, deg, degMin, degSec);
}
////////////////////////////////////////////////////////////////////////////////
void ac::degminsec2hourminsec(const int deg,
                          const int degMin,
                          const int degSec,
                          int &hour,
                          int &hourMin,
                          int &hourSec)
{
    double rad = (deg + degMin / 60 + degSec / 3600) * __deg2rad;
    rad2hourminsec( rad, hour, hourMin, hourSec );
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    //ROTATION
////////////////////////////////////////////////////////////////////////////////
void ac::axes2horiz(const double axesAzimuth,
                const double axesElevation,
                const double orientAzimuth,
                const double orientElevation,
                double &horizAzimuth,
                double &horizElevation)
{
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, (0 - astro2math(orientAzimuth)) * __rad2deg);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, (0 - orientElevation) * __rad2deg);
    QQuaternion q = qz * qy;
    double x, y, z;
    spherical2cartesian(astro2math(axesAzimuth), axesElevation, x, y, z);
    QVector3D r0 = q.rotatedVector(QVector3D(x, y, z));
    cartesian2spherical(r0.x(), r0.y(), r0.z(), horizAzimuth, horizElevation);
    horizAzimuth = math2astro(horizAzimuth);
}
////////////////////////////////////////////////////////////////////////////////
void ac::horiz2axes(const double horizAzimuth,
                const double horizElevation,
                const double orientAzimuth,
                const double orientElevation,
                double &axesAzimuth,
                double &axesElevation)
{
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, astro2math(orientAzimuth) * __rad2deg);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, orientElevation * __rad2deg);
    QQuaternion q  = qy * qz;
    double x, y, z;
    spherical2cartesian(astro2math(horizAzimuth), horizElevation, x, y, z);
    QVector3D r = q.rotatedVector(QVector3D(x, y, z));
    cartesian2spherical(r.x(), r.y(), r.z(), axesAzimuth, axesElevation);
    axesAzimuth = math2astro(axesAzimuth);
}
////////////////////////////////////////////////////////////////////////////////
void ac::horiz2iieqt(const double horizAzimuth,
                 const double horizElevation,
                 const double localSiderealTime,
                 const double latitude,
                 double &alpha,
                 double &delta)
{
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, localSiderealTime * __rad2deg);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, (__pi2 - latitude) * __rad2deg);
    QQuaternion q = qz * qy;
    double x, y, z;
    spherical2cartesian(astro2math(horizAzimuth), horizElevation, x, y, z);
    QVector3D r = q.rotatedVector(QVector3D(x, y, z));
    cartesian2spherical(r.x(), r.y(), r.z(), alpha, delta);
    alpha = shiftPlus(alpha);
}
////////////////////////////////////////////////////////////////////////////////
void ac::iieqt2horiz(const double alpha,
                     const double delta,
                     const double localSiderealTime,
                     const double latitude,
                     double &horizAzimuth,
                     double &horizElevation)
{
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, (0 - localSiderealTime) * __rad2deg);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, (latitude - __pi2) * __rad2deg);
    QQuaternion q = qy * qz;
    double x, y, z;
    spherical2cartesian(shiftMinus(alpha), delta, x, y, z);
    QVector3D r = q.rotatedVector(QVector3D(x, y, z));
    cartesian2spherical(r.x(), r.y(), r.z(), horizAzimuth, horizElevation);
    horizAzimuth = math2astro(horizAzimuth);
}
////////////////////////////////////////////////////////////////////////////////
void ac::screen2horiz(const double centerAzimuth,
                  const double centerElevation,
                  const double screenX,
                  const double screenY,
                  double &horizAzimuth,
                  double &horizElevation)
{
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, astro2math(centerAzimuth) * __rad2deg);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, (0 - centerElevation) * __rad2deg);
    QQuaternion q  = qz * qy;
    double x, y, z;
    spherical2cartesian(screenX , screenY , x, y, z);
    QVector3D rh = q.rotatedVector(QVector3D(x, y, z));
    cartesian2spherical(rh.x(), rh.y(), rh.z(), horizAzimuth, horizElevation);
    horizAzimuth = math2astro(horizAzimuth);
}
////////////////////////////////////////////////////////////////////////////////
void ac::horiz2screen(const double centerAzimuth,
                      const double centerElevation,
                      const double horizAzimuth,
                      const double horizElevation,
                      double &screenX,
                      double &screenY)
{
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, (0 - astro2math(centerAzimuth)) * __rad2deg);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, centerElevation * __rad2deg);
    QQuaternion q = qy * qz;
    double x, y, z;
    spherical2cartesian(astro2math(horizAzimuth), horizElevation, x, y, z);
    QVector3D rs = q.rotatedVector(QVector3D(x, y, z));
    cartesian2spherical(rs.x(), rs.y(), rs.z(), screenX, screenY);
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    //SCREEN
////////////////////////////////////////////////////////////////////////////////
//no field rotation !
void ac::screenAngles2screenPoint(const double angleX,
                                  const double angleY,
                                  const double starDelta,
                                  const QSizeF &field,
                                  const QSize  &screen,
                                  double &x,
                                  double &y)
{
    QPointF rad2pix(screen.width() / field.width(),
                    screen.height() / field.height());
    double x0 = angleX * rad2pix.x() / qAbs(qCos(starDelta));
    double y0 = angleY * rad2pix.y();
    x = -x0 + (screen.width() / 2);
    y = -y0 + (screen.height() / 2);
}
////////////////////////////////////////////////////////////////////////////////
//no field rotation !
void ac::screenPoint2screenAngles(const double x,
                                  const double y,
                                  const double starDelta,
                                  const QSizeF &field,
                                  const QSize &screen,
                                  double &angleX,
                                  double &angleY)
{
    double x0 = -x + (screen.width() / 2);
    double y0 = -y + (screen.height() / 2);
    QPointF pix2rad(field.width() / screen.width(),
                    field.height() / screen.height());
    angleX = x0 * pix2rad.x() * qAbs(qCos(starDelta));
    angleY = y0 * pix2rad.y();
}
////////////////////////////////////////////////////////////////////////////////
double ac::calcStarRadius(const double magnitude)
{
    const double magicNumber = 5.0;
    const double radiusPar = 55.0;
    double expfun = qExp( -magnitude / magicNumber );
    return ac::round(radiusPar * expfun);
}
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
