#include "astrometry.h"
/////////////////////////////////////////////////////////////////////////////////////
void deleteNearStars(ArtifactVector &a,
                     const double eps)
{
    if(a.empty())    return;
    ArtifactVector::iterator it1 = a.begin();
    while(it1 < a.end())
    {
        ArtifactVector::iterator it2 = a.begin();
        while(it2 < a.end())
        {
            if(it1 == it2)
            {
                ++it2;
                continue;
            }
            if(art::isEqual(*it1, *it2, eps))
            {
                it2 = a.erase(it2);
            }
            else
            {
                ++it2;
            }
        }
        ++it1;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void precook(ArtifactVector &picStars,
             ArtifactVector &catStars,
             const QSize  &screen,
             const int    maxStarQuantity,
             const double nearStarDist)
{
    QPointF screenCenter(screen.width() / 2, screen.height() / 2);
    art::selectOnCircle(picStars,
                        screenCenter,
                        screenCenter.y());
    art::selectOnCircle(catStars,
                        screenCenter,
                        screenCenter.y());
    art::cutoff(picStars, maxStarQuantity);
    art::cutoff(catStars, maxStarQuantity);

    deleteNearStars(picStars, nearStarDist);
    deleteNearStars(catStars, nearStarDist);

    if(picStars.size() > catStars.size())
    {
        art::cutoff(picStars, catStars.size());
    }
    if(catStars.size() > picStars.size())
    {
        art::cutoff(catStars, picStars.size());
    }
}
/////////////////////////////////////////////////////////////////////////////////////
int astrometry::equate(ArtifactVector &picStars,
                       ArtifactVector &catStars,
                       const QSize  &screen,
                       const double similarEps,
                       const double nearStarDist,
                       const int maxStarQuantity,
                       const int minEquatedStarQuantity,
                       METHOD    method)
{
    if(picStars.size() < _minStarQuantity)     return TOO_LESS_RAW_STARS;
    if(catStars.size() < _minStarQuantity)     return TOO_LESS_RAW_STARS;

    int starQuantity;
    if((method == SIMTRI) && (maxStarQuantity > simtri::_maxStarQuantityForSimtriMethod))
    {
        starQuantity = simtri::_maxStarQuantityForSimtriMethod;
    }
    else
    {
        starQuantity = maxStarQuantity;
    }

    precook(picStars,
            catStars,
            screen,
            starQuantity,
            nearStarDist);

    switch(method)
    {
    case SIMTRI:
        return simtri::equate(picStars,
                              catStars,
                              similarEps,
                              minEquatedStarQuantity);
        break;

    case FREEVEC:
        return freevec::equate(picStars,
                               catStars,
                               screen,
                               similarEps,
                               minEquatedStarQuantity);
        break;

    default:
        return UNKNOWN_ERROR;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void astrometry::catStar2screenStar(const Star   &s,
                                    Artifact     &a,
                                    const TelPos &t,
                                    const QSizeF &field,
                                    const QSize  &screen)
{
    double starAzimuth, starElevation;
    double starAngleX, starAngleY;
    ac::iieqt2horiz(s.alpha(), s.delta(),
                    t.LST, t.latitude,
                    starAzimuth, starElevation);
    ac::horiz2screenAngles(t.azHoriz, t.elHoriz,
                           starAzimuth, starElevation,
                           starAngleX, starAngleY);
    double x, y;
    ac::screenAngles2screenPoint(starAngleX,
                                 starAngleY,
                                 s.delta(),
                                 field,
                                 screen,
                                 x,
                                 y);
    a.setCenter(QPointF(x, y));
    a.setMagnitude(s.magnitude());
}
////////////////////////////////////////////////////////////////////////////////
void astrometry::screenStar2catStar(const Artifact &a,
                                    Star           &s,
                                    const TelPos   &t,
                                    const QSizeF   &field,
                                    const QSize    &screen)
{
    double xAng, yAng;
    double azHoriz, elHoriz;
    double alpha, delta;
    ac::screenPoint2screenAngles(a.center().x(),
                                 a.center().y(),
                                 t.delta,
                                 field,
                                 screen,
                                 xAng,
                                 yAng);
    ac::screenAngles2horiz(t.azHoriz,
                           t.elHoriz,
                           xAng,
                           yAng,
                           azHoriz,
                           elHoriz);
    ac::horiz2iieqt(azHoriz,
                    elHoriz,
                    t.LST,
                    t.latitude,
                    alpha,
                    delta);
    s.setAlpha(alpha);
    s.setDelta(delta);
}
////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
