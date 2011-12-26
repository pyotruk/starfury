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
void astrometry::precook(ArtifactVector &picStars,
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
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
