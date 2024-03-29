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
    if(angle > ac::_pi)
        return (angle - ac::_2pi);
    else
        return angle;
}
////////////////////////////////////////////////////////////////////////////////
double shiftPlus(const double angle)
/* -180..+180  -->  0..360 */
{
    if(angle < 0)
        return (ac::_2pi + angle);
    else
        return angle;
}
////////////////////////////////////////////////////////////////////////////////
double astro2math(const double azimuth)
/* 0..360 ����� �� (��������������� ������) --->  -180..+180 ������ �� */
{
  return (0 - shiftMinus(azimuth));
}
////////////////////////////////////////////////////////////////////////////////
double math2astro(const double azimuth)
/* -180..+180 ������ ��  --->  0..360 ����� �� (��������������� ������) */
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
void ac::rad2dms(const double rad,
                 int &deg,
                 int &min,
                 int &sec)
{
    double degAng = rad * _rad2deg;
    deg = qFloor(degAng);
    double foo = frac(degAng) * 60;
    min = qFloor(foo);
    sec = qFloor(frac(foo) * 60);
}
////////////////////////////////////////////////////////////////////////////////
void ac::rad2hms(const double rad,
                 int &hour,
                 int &min,
                 int &sec)
{
    double hourAng = rad * _rad2hour;
    hour = qFloor(hourAng);
    double foo = frac(hourAng) * 60;
    min = qFloor(foo);
    sec = qFloor(frac(foo) * 60);
}
////////////////////////////////////////////////////////////////////////////////
void ac::hms2dms(const int hour,
                 const int hourMin,
                 const int hourSec,
                 int &deg,
                 int &degMin,
                 int &degSec)
{
    double rad = (hour + hourMin / 60 + hourSec / 3600) * _hour2rad;
    rad2dms(rad, deg, degMin, degSec);
}
////////////////////////////////////////////////////////////////////////////////
void ac::dms2hms(const int deg,
                 const int degMin,
                 const int degSec,
                 int &hour,
                 int &hourMin,
                 int &hourSec)
{
    double rad = (deg + degMin / 60 + degSec / 3600) * _deg2rad;
    rad2hms( rad, hour, hourMin, hourSec );
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
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, (0 - astro2math(orientAzimuth)) * _rad2deg);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, (0 - orientElevation) * _rad2deg);
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
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, astro2math(orientAzimuth) * _rad2deg);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, orientElevation * _rad2deg);
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
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, localSiderealTime * _rad2deg);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, (_pi2 - latitude) * _rad2deg);
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
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, (0 - localSiderealTime) * _rad2deg);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, (latitude - _pi2) * _rad2deg);
    QQuaternion q = qy * qz;
    double x, y, z;
    spherical2cartesian(shiftMinus(alpha), delta, x, y, z);
    QVector3D r = q.rotatedVector(QVector3D(x, y, z));
    cartesian2spherical(r.x(), r.y(), r.z(), horizAzimuth, horizElevation);
    horizAzimuth = math2astro(horizAzimuth);
}
////////////////////////////////////////////////////////////////////////////////
void ac::screenAngles2horiz(const double screenCenterHorizAzimuth,
                            const double screenCenterHorizElevation,
                            const double screenAngleX,
                            const double screenAngleY,
                            double &horizAzimuth,
                            double &horizElevation)
{
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, astro2math(screenCenterHorizAzimuth) * _rad2deg);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, (0 - screenCenterHorizElevation) * _rad2deg);
    QQuaternion q  = qz * qy;
    double x, y, z;
    spherical2cartesian(screenAngleX , screenAngleY, x, y, z);
    QVector3D rh = q.rotatedVector(QVector3D(x, y, z));
    cartesian2spherical(rh.x(), rh.y(), rh.z(), horizAzimuth, horizElevation);
    horizAzimuth = math2astro(horizAzimuth);
}
////////////////////////////////////////////////////////////////////////////////
void ac::horiz2screenAngles(const double screenCenterHorizAzimuth,
                            const double screenCenterHorizElevation,
                            const double horizAzimuth,
                            const double horizElevation,
                            double &screenAngleX,
                            double &screenAngleY)
{
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, (0 - astro2math(screenCenterHorizAzimuth)) * _rad2deg);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, screenCenterHorizElevation * _rad2deg);
    QQuaternion q = qy * qz;
    double x, y, z;
    spherical2cartesian(astro2math(horizAzimuth), horizElevation, x, y, z);
    QVector3D rs = q.rotatedVector(QVector3D(x, y, z));
    cartesian2spherical(rs.x(), rs.y(), rs.z(), screenAngleX, screenAngleY);
}
////////////////////////////////////////////////////////////////////////////////
void ac::screenAngles2iieqt(const double screenAngleX,
                            const double screenAngleY,
                            const double screenCenterHorizAzimuth,
                            const double screenCenterHorizElevation,
                            const double localSiderealTime,
                            const double latitude,
                            double &alpha,
                            double &delta)
{
    double horizAz, horizEl;
    screenAngles2horiz(screenCenterHorizAzimuth,
                       screenCenterHorizElevation,
                       screenAngleX,
                       screenAngleY,
                       horizAz,
                       horizEl);
    horiz2iieqt(horizAz,
                horizEl,
                localSiderealTime,
                latitude,
                alpha,
                delta);
}
////////////////////////////////////////////////////////////////////////////////
void ac::iieqt2screenAngles(const double alpha,
                            const double delta,
                            const double localSiderealTime,
                            const double latitude,
                            const double screenCenterHorizAzimuth,
                            const double screenCenterHorizElevation,
                            double &screenAngleX,
                            double &screenAngleY)
{
    double horizAz, horizEl;
    iieqt2horiz(alpha,
                delta,
                localSiderealTime,
                latitude,
                horizAz,
                horizEl);
    horiz2screenAngles(screenCenterHorizAzimuth,
                       screenCenterHorizElevation,
                       horizAz,
                       horizEl,
                       screenAngleX,
                       screenAngleY);
}
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    //SCREEN
////////////////////////////////////////////////////////////////////////////////
//no field rotation !
void ac::screenAngles2screenPoint(const double screenAngleX,
                                  const double screenAngleY,
                                  const double starDelta,
                                  const QSizeF &field,
                                  const QSize &screen,
                                  double &x, double &y)
{
    QPointF rad2pix(screen.width() / field.width(),
                    screen.height() / field.height());
    double x0 = screenAngleX * rad2pix.x() / qAbs(qCos(starDelta));
    double y0 = screenAngleY * rad2pix.y();
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
                                  double &screenAngleX,
                                  double &screenAngleY)
{
    double x0 = -x + (screen.width() / 2);
    double y0 = -y + (screen.height() / 2);
    QPointF pix2rad(field.width() / screen.width(),
                    field.height() / screen.height());
    screenAngleX = x0 * pix2rad.x() * qAbs(qCos(starDelta));
    screenAngleY = y0 * pix2rad.y();
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
