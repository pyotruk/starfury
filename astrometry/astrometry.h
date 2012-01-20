#ifndef ASTROMETRY_H
#define ASTROMETRY_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QSize>
#include <QSizeF>
#include <QPointF>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/telescope.h"
#include "boxes/artifact.h"
#include "boxes/star.h"
#include "math/astrocalc.h"
#include "astrometry/simtri/triangle.h"
#include "astrometry/freevec/freevec.h"
/////////////////////////////////////////////////////////////////////////////////////
namespace astrometry
{
//return values
enum RETURN_VALUES {SUCCESS = 0,
                    TOO_LESS_RAW_STARS     = -1,  //less than __minStarQuantity
                    TOO_LESS_EQUATED_STARS = -2,  //less than minEquatedStarQuantity
                    UNKNOWN_ERROR          = -3}; //when switch default (it is impossible)
//methods
enum METHOD {SIMTRI = 0, FREEVEC = 1};
//
static const int _minStarQuantity = 3;
//
int equate(ArtifactVector &picStars,
           ArtifactVector &catStars,
           const QSize    &screen,
           const double    similarEps,
           const double    nearStarDist,
           const int       maxStarQuantity,
           const int       minEquatedStarQuantity,
           METHOD          method);
void catStar2screenStar(const Star   &s,
                        Artifact     &a,
                        const TelPos &t,
                        const QSizeF &field,
                        const QSize  &screen);
void screenStar2catStar(const Artifact &a,
                        Star           &s,
                        const TelPos   &t,
                        const QSizeF   &field,
                        const QSize    &screen);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // ASTROMETRY_H
