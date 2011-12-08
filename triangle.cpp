#include "triangle.h"
/////////////////////////////////////////////////////////////////////////////////////
bool tri::isEqual(const ArtifactTriangle &t1,
                  const ArtifactTriangle &t2,
                  const double eps)
{
    QPoint p1[3]; //массив точек
    p1[0] = t1.t()[0]->center();
    p1[1] = t1.t()[1]->center();
    p1[2] = t1.t()[2]->center();

    QPoint p2[3]; //массив точек
    p2[0] = t2.t()[0]->center();
    p2[1] = t2.t()[1]->center();
    p2[2] = t2.t()[2]->center();

    int eq = 0;

    for(int i=0; i < 3; ++i)
    {
        for(int j=0; j < 3; ++j)
        {
            if(ac::isEqual(p1[i], p2[j], eps))
            {
               eq++;
               continue;
            }
        }
    }

    if(eq == 3)     return true;
    else            return false;
}
/////////////////////////////////////////////////////////////////////////////////////
bool tri::isSimilar(const ArtifactTriangle &t1,
                    const ArtifactTriangle &t2,
                    const double eps,
                    SimPath& sim)
{
    double s1[3]; //массив сторон
    s1[0] = ac::calcDistance(t1.t()[0]->center(), t1.t()[1]->center());
    s1[1] = ac::calcDistance(t1.t()[0]->center(), t1.t()[2]->center());
    s1[2] = ac::calcDistance(t1.t()[1]->center(), t1.t()[2]->center());

    double s2[3]; //массив сторон
    s2[0] = ac::calcDistance(t2.t()[0]->center(), t2.t()[1]->center());
    s2[1] = ac::calcDistance(t2.t()[0]->center(), t2.t()[2]->center());
    s2[2] = ac::calcDistance(t2.t()[1]->center(), t2.t()[2]->center());

    double r1[3]; //массив отношений сторон
    r1[0] = s1[0] / s2[0];
    r1[1] = s1[0] / s2[1];
    r1[2] = s1[0] / s2[2];

    double r2[3]; //массив отношений сторон
    r1[0] = s1[1] / s2[0];
    r1[1] = s1[1] / s2[1];
    r1[2] = s1[1] / s2[2];

    double r3[3]; //массив отношений сторон
    r1[0] = s1[2] / s2[0];
    r1[1] = s1[2] / s2[1];
    r1[2] = s1[2] / s2[2];

    int i = 0;
    int j = 0;
    for(; i < 3; ++i)
    {
        for(; j < 3; ++j)
        {
            if(i == j)
                continue;
            if(qAbs(r1[i] - r2[j]) < eps)
            {
                sim[0] = i;
                sim[1] = j;
                goto  found;
            }
        }
    }
    return false;

found:
    for(int k=0; k < 3; ++k)
    {
        if((k == i) || (k == j))
            continue;
        if(qAbs(r3[k] - r2[j]) < eps)
        {
            sim[2] = k;
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
void tri::cookTriangles(ArtifactVector &a,
                        TriangleVector &t)
{
    t.clear();
    ArtifactVector::iterator i = a.begin();
    ArtifactVector::iterator j = a.begin();
    ArtifactVector::iterator k = a.begin();
    for(; i < a.end(); ++i)
    {
        for(; j < a.end(); ++j)
        {
            if(j == i)    continue;
            for(; k < a.end(); ++k)
            {
                if((k == i) || (k == j))    continue;
                t.push_back(ArtifactTriangle(i, j, k));
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void tri::deleteEqual(TriangleVector &t,
                      const double eps)
{
    TriangleVector::iterator i = t.begin();
    TriangleVector::iterator j = t.begin();
    for(; i < t.end(); ++i)
    {
        for(; j < t.end(); ++j)
        {
            if(j == i)    continue;
            if(tri::isEqual(*i, *j, eps))
            {
                t.erase(j);
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void tri::cookEquatedArtifacts(const TriangleVector &t1,
                               const TriangleVector &t2,
                               const double eps,
                               ArtifactVector &a1,
                               ArtifactVector &a2)
{
    SimPath sim;
    a1.clear();
    a2.clear();
    TriangleVector::const_iterator it1 = t1.constBegin();
    TriangleVector::const_iterator it2 = t2.constBegin();
    for(; it1 < t1.constEnd(); ++it1)
    {
        for(; it2 < t2.constEnd(); ++it2)
        {
            if(tri::isSimilar(*it1, *it2, eps, sim))
            {
                for(int i = 0; i < 3; ++i)
                {
                    a1.push_back(*(it1->t()[i]));
                    a2.push_back(*(it2->t()[sim[i]]));
                }
                break;
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
