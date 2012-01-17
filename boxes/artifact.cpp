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
QDebug operator<<(QDebug d, const Artifact &a)
{
    d << "x = " << a.center().x()
      << "  y = " << a.center().y()
      << "  magn = " << a.magnitude();
    return d;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void art::selectOnCircle(ArtifactVector &a,     //удаление артефактов, которые не попали на круг
                         const QPointF &center,
                         const double radius)
{
    if(a.empty())    return;
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
void art::cutoff(ArtifactVector &a,     //обрезка вектора до нужной длины
                 const int newLength)       //если newLenght больше текущей длины, то ничего не происходит
{
    if(a.empty())    return;
    int dif = a.size() - newLength;
    if(dif > 0)     a.erase(a.end() - dif, a.end());
}
/////////////////////////////////////////////////////////////////////////////////////
bool art::isEqual(const Artifact &a1,   //проверка артефактов на равенство (совпадение центров масс)
                  const Artifact &a2,
                  const double eps)
{
    return  ac::isEqual(a1.center(), a2.center(), eps);
}
/////////////////////////////////////////////////////////////////////////////////////
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
void art::deleteTargets(ArtifactVector &stars,
                        const ArtifactVector &targets)
{
    if(stars.empty())    return;
    if(targets.empty())   return;
    const double eps = 2;
    double dist;
    double minDist = 1000000;
    int targetIndex;
    QPointF targetCenter = targets.front().center();
    ArtifactVector::iterator it = stars.begin();

    qDebug() << "art::deleteTargets() says: Hi!"
             << "   stars.begin() = " << it << "\n"
             << "   targetCenter = " << targetCenter << "\n"
             << "   targets.size() = " << targets.size();

    for(int i=0; it < stars.end(); ++it, ++i)
    {
        dist = ac::calcDistance(targetCenter, it->center());

        qDebug() << "art::deleteTargets() says: i`m inside FOR-loop" << "\n"
                 << "   it = " << it << "\n"
                 << "   stars[i] = " << *it << "\n"
                 << "   dist = " << dist;

        if(dist < minDist)
        {
            minDist = dist;
            targetIndex = i;
            if(minDist < eps)   break;
        }
    }
    if(targetIndex >= stars.size())
    {
        qDebug() << "art::deleteTargets() says: targetIndex >= stars.size()" << "\n"
                 << "   stars.size() = " << stars.size() << "\n"
                 << "   stars[0] = " << stars.front() << "\n"
                 << "   targetIndex = " << targetIndex << "\n"
                 << "   minDist = " << minDist;
    }
    stars.remove(targetIndex);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
