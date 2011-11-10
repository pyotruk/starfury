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
double ac::calcDistance(const QPoint &p1,
                        const QPoint &p2)
{
    return qSqrt( qPow((p1.x() - p2.x()), 2) +
                  qPow((p1.y() - p2.y()), 2) );
}
////////////////////////////////////////////////////////////////////////////////
double shiftMinus(const double angle)
/* 0..360  -->  -180..+180 */
{
    if(angle > PI)
        return (angle - TWOPI);
    else
        return angle;
}
////////////////////////////////////////////////////////////////////////////////
double shiftPlus(const double angle)
/* -180..+180  -->  0..360 */
{
    if(angle < 0)
        return (TWOPI + angle);
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
void ac::rad2degminsec(const double rad,
                   int &deg,
                   int &min,
                   int &sec)
{
    double degAng = rad * RAD2DEG;
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
    double hourAng = rad * RAD2HOUR;
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
    double rad = (hour + hourMin / 60 + hourSec / 3600) * HOUR2RAD;
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
    double rad = (deg + degMin / 60 + degSec / 3600) * DEG2RAD;
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
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, (0 - astro2math(orientAzimuth)));
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, (0 - orientElevation));
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
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, astro2math(orientAzimuth));
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, orientElevation);
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
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, localSiderealTime);
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, (PI2 - latitude));
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
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, (0 - localSiderealTime));
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, (latitude - PI2));
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
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, astro2math(centerAzimuth));
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, (0 - centerElevation));
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
    QQuaternion qz = QQuaternion::fromAxisAndAngle(0, 0, 1, (0 - astro2math(centerAzimuth)));
    QQuaternion qy = QQuaternion::fromAxisAndAngle(0, 1, 0, centerElevation);
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
void ac::screenAngles2screenPoint(const double angleX, //no field rotation !
                                    const double angleY,
                                    const double starDelta,
                                    const QSizeF &field,
                                    const QSize  &screen,
                                    int &x,
                                    int &y)
{
    QPoint rad2pix(screen.width() / field.width(),
                   screen.height() / field.height());
    int x0 = round(angleX * rad2pix.x() / qAbs(qCos(starDelta)));
    int y0 = round(angleY * rad2pix.y());
    x = -x0 + (screen.width() / 2);
    y = -y0 + (screen.height() / 2);
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
