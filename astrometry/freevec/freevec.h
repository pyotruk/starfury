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
#include "astrometry/astrometry.h"
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
namespace freevec
{
static const double _distDiffEps = 1.0; /* расстояния считаются одинаковыми
                                           при разнице < _distDiffEps */
int equate(ArtifactVector &picStars,
           ArtifactVector &catStars,
           const QSize    &screen,
           const double    similarEps,
           const int       minEquatedStarQuantity);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // IDENTIFIER_H
