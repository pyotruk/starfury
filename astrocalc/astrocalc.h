#ifndef ASTROCALC_H
#define ASTROCALC_H
////////////////////////////////////////////////////////////////////////////////
#include <QVector3D>
#include <QQuaternion>
#include <qmath.h>
#include <QSize>
#include <QSizeF>
#include <QPoint>
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define PI          3.1415926535897932384626433832795
#define TWOPI       6.283185307179586476925286766559
#define PI2         1.5707963267948966192313216916398
#define RAD2DEG     57.295779513082320876798154814105
#define DEG2RAD     0.017453292519943295769236907684886
#define HOUR2DEG    15.0
#define DEG2HOUR    0.066666666666666666666666666666667
#define HOUR2RAD    0.26179938779914943653855361527329
#define RAD2HOUR    3.8197186342054880584532103209403
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
namespace ac
{
    //UTILS
////////////////////////////////////////////////////////////////////////////////
double frac(const double x);
int round(const double x);
////////////////////////////////////////////////////////////////////////////////
double calcDistance(const QPoint &p1,
                    const QPoint &p2);
bool isEqual(const QPoint &p1,
             const QPoint &p2,
             const double eps);
////////////////////////////////////////////////////////////////////////////////
void rad2degminsec(const double rad,
                   int &deg,
                   int &min,
                   int &sec);
void rad2hourminsec(const double rad,
                    int &hour,
                    int &min,
                    int &sec);
void hourminsec2degminsec(const int hour,
                          const int hourMin,
                          const int hourSec,
                          int &deg,
                          int &degMin,
                          int &degSec);
void degminsec2hourminsec(const int deg,
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
void screen2horiz(const double centerAzimuth,
                  const double centerElevation,
                  const double screenX,
                  const double screenY,
                  double &horizAzimuth,
                  double &horizElevation);
void horiz2screen(const double centerAzimuth,
                  const double centerElevation,
                  const double horizAzimuth,
                  const double horizElevation,
                  double &screenX,
                  double &screenY);
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    //SCREEN
////////////////////////////////////////////////////////////////////////////////
void screenAngles2screenPoint(const double angleX, //no field rotation !
                              const double angleY,
                              const double starDelta,
                              const QSizeF &field,
                              const QSize  &screen,
                              int &x,
                              int &y);
////////////////////////////////////////////////////////////////////////////////
double calcStarRadius(const double magnitude);
////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif // ASTROCALC_H
