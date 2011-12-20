#include "artifact.h"
/////////////////////////////////////////////////////////////////////////////////////
Artifact::Artifact() :
    _center(0, 0),
    _magnitude(0)
{}
/////////////////////////////////////////////////////////////////////////////////////
Artifact::Artifact(const Artifact &a)
{
    this->_center = a._center;
    this->_magnitude = a._magnitude;
}
/////////////////////////////////////////////////////////////////////////////////////
Artifact::Artifact(const double x,
                   const double y,
                   const double magn) :
    _center(x, y),
    _magnitude(magn)
{}
/////////////////////////////////////////////////////////////////////////////////////
Artifact& Artifact::operator =(const Artifact &a)
{
    if(this != &a)
    {
        this->_center = a._center;
        this->_magnitude = a._magnitude;
    }
    return *this;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
QDebug& operator <<(QDebug &stream, const Artifact &a)
{
    stream << "x = " << a.center().x()
           << "  y = " << a.center().y()
           << "  magn = " << a.magnitude();
    return stream;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void art::selectOnCircle(ArtifactVector &a,
                         const QPointF &center,
                         const double radius)
{
    double dist;
    ArtifactVector::iterator it = a.begin();
    while(it < a.end())
    {
        dist = ac::calcDistance(center, it->center());
        if(dist > radius)
        {
            it = a.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void art::cutoff(ArtifactVector &a,
                 const int newLength)
{
    int dif = a.size() - newLength;
    if(dif > 0)     a.erase(a.end() - dif, a.end());
}
/////////////////////////////////////////////////////////////////////////////////////
bool art::isEqual(const Artifact &a1,
                  const Artifact &a2,
                  const double eps)
{
    return  ac::isEqual(a1.center(), a2.center(), eps);
}
/////////////////////////////////////////////////////////////////////////////////////
void art::deleteEqual(ArtifactVector &a,
                      const double eps)
{
    for(ArtifactVector::iterator
        i = a.begin(); i < a.end(); ++i)
    {
        for(ArtifactVector::iterator
            j = a.begin(); j < a.end(); ++j)
        {
            if(j == i)    continue;
            if(art::isEqual(*i, *j, eps))
            {
                a.erase(j);
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
bool art::isContain(const ArtifactVector &v,
                    const Artifact &a,
                    const double eps)
{
    for(ArtifactVector::const_iterator
        it = v.constBegin(); it < v.constEnd(); ++it)
    {
        if(art::isEqual(*it, a, eps))
        {
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
