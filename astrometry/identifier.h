#ifndef IDENTIFIER_H
#define IDENTIFIER_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QtAlgorithms>
#include <QVector>
#include <QPointF>
#include <QSize>
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/artifact.h"
#include "math/astrocalc.h"
/////////////////////////////////////////////////////////////////////////////////////
class ArtifactPair
{
public:
    explicit ArtifactPair();
    explicit ArtifactPair(const Artifact *refStar,
                          const Artifact &anyStar);
    bool operator ==(const ArtifactPair &other) const {return this->_dist == other._dist;}
    bool operator !=(const ArtifactPair &other) const {return this->_dist != other._dist;}
    bool operator <(const ArtifactPair &other)  const {return this->_dist <  other._dist;}
    const Artifact& star() const {return _star;}
    const Artifact* refStar() const {return _refStar;}
    double dist() const {return _dist;}
private:
    const Artifact *_refStar;
    Artifact        _star;
    double          _dist;
};
/////////////////////////////////////////////////////////////////////////////////////
typedef QVector<ArtifactPair> ArtifactPairVector;
/////////////////////////////////////////////////////////////////////////////////////
namespace identifier
{
//return values
static const int __SUCCESS                = 0;
static const int __TOO_LESS_RAW_STARS     = -1; //less than __minStarQuantity
static const int __TOO_LESS_EQUATED_STARS = -2; //less than minEquatedStarQuantity
//
static const int __minStarQuantity = 3;
int equate(ArtifactVector &picStars,
           ArtifactVector &catStars,
           const QSize    &screen,
           const double    similarEps,
           const double    nearStarDist,
           const int       maxStarQuantity,
           const int       minEquatedStarQuantity);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // IDENTIFIER_H
