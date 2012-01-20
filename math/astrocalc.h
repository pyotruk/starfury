#ifndef ASTROCALC_H
#define ASTROCALC_H
////////////////////////////////////////////////////////////////////////////////
#include <QVector3D>
#include <QQuaternion>
#include <qmath.h>
#include <QSize>
#include <QSizeF>
#include <QPointF>
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace ac
{
static const double _pi = 3.1415926535897932384626433832795;
static const double _2pi = 6.283185307179586476925286766559;
static const double _pi2 = 1.5707963267948966192313216916398;
static const double _rad2deg = 57.295779513082320876798154814105;
static const double _deg2rad = 0.017453292519943295769236907684886;
static const double _hour2deg = 15.0;
static const double _deg2hour = 0.066666666666666666666666666666667;
static const double _hour2rad = 0.26179938779914943653855361527329;
static const double _rad2hour = 3.8197186342054880584532103209403;

    //UTILS
////////////////////////////////////////////////////////////////////////////////
double frac(const double x);
int round(const double x);
////////////////////////////////////////////////////////////////////////////////
double calcDistance(const QPointF &p1,
                    const QPointF &p2);
bool isEqual(const QPointF &p1,
             const QPointF &p2,
             const double eps);
////////////////////////////////////////////////////////////////////////////////
void rad2dms(const double rad,
             int &deg,
             int &min,
             int &sec);
void rad2hms(const double rad,
             int &hour,
             int &min,
             int &sec);
void hms2dms(const int hour,
             const int hourMin,
             const int hourSec,
             int &deg,
             int &degMin,
             int &degSec);
void dms2hms(const int deg,
             const int degMin,
             const int degSec,
             int &hour,
             int &hourMin,
             int &hourSec);
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    //ROTATION
////////////////////////////////////////////////////////////////////////////////
void axes2horiz(const double axesAzimuth,
                const double axesElevation,
                const double orientAzimuth,
                const double orientElevation,
                double &horizAzimuth,
                double &horizElevation);
void horiz2axes(const double horizAzimuth,
                const double horizElevation,
                const double orientAzimuth,
                const double orientElevation,
                double &axesAzimuth,
                double &axesElevation);
////////////////////////////////////////////////////////////////////////////////
void horiz2iieqt(const double horizAzimuth,
                 const double horizElevation,
                 const double localSiderealTime,
                 const double latitude,
                 double &alpha,
                 double &delta);
void iieqt2horiz(const double alpha,
                 const double delta,
                 const double localSiderealTime,
                 const double latitude,
                 double &horizAzimuth,
                 double &horizElevation);
////////////////////////////////////////////////////////////////////////////////
void screenAngles2horiz(const double screenCenterHorizAzimuth,
                        const double screenCenterHorizElevation,
                        const double screenAngleX,
                        const double screenAngleY,
                        double &horizAzimuth,
                        double &horizElevation);
void horiz2screenAngles(const double screenCenterHorizAzimuth,
                        const double screenCenterHorizElevation,
                        const double horizAzimuth,
                        const double horizElevation,
                        double &screenAngleX,
                        double &screenAngleY);
////////////////////////////////////////////////////////////////////////////////
void screenAngles2iieqt(const double screenAngleX,
                        const double screenAngleY,
                        const double screenCenterHorizAzimuth,
                        const double screenCenterHorizElevation,
                        const double localSiderealTime,
                        const double latitude,
                        double &alpha,
                        double &delta);
void iieqt2screenAngles(const double alpha,
                        const double delta,
                        const double localSiderealTime,
                        const double latitude,
                        const double screenCenterHorizAzimuth,
                        const double screenCenterHorizElevation,
                        double &screenAngleX,
                        double &screenAngleY);
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    //SCREEN
////////////////////////////////////////////////////////////////////////////////
 //no field rotation !
void screenAngles2screenPoint(const double screenAngleX,
                              const double screenAngleY,
                              const double starDelta,
                              const QSizeF &field,
                              const QSize  &screen,
                              double &x,
                              double &y);
////////////////////////////////////////////////////////////////////////////////
 //no field rotation !
void screenPoint2screenAngles(const double x,
                              const double y,
                              const double starDelta,
                              const QSizeF &field,
                              const QSize  &screen,
                              double &screenAngleX,
                              double &screenAngleY);
////////////////////////////////////////////////////////////////////////////////
double calcStarRadius(const double magnitude);
////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif // ASTROCALC_H
