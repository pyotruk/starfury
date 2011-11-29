#include "arttriangle.h"
/////////////////////////////////////////////////////////////////////////////////////
ArtifactTriangle::ArtifactTriangle(Artifact *a,
                                   Artifact *b,
                                   Artifact *c) :
    _a(a),
    _b(b),
    _c(c)
{}
/////////////////////////////////////////////////////////////////////////////////////
bool ArtifactTriangle::isValid()
{
    if((_a != 0) && (_b != 0) && (_c != 0))     return true;
    else                                        return false;
}
/////////////////////////////////////////////////////////////////////////////////////
bool ArtifactTriangle::isEqualTo(const ArtifactTriangle &t,
                                 const double eps)
{
    QPoint p1[3]; //массив точек
    p1[0] = this->_a->center();
    p1[1] = this->_b->center();
    p1[2] = this->_c->center();

    QPoint p2[3]; //массив точек
    p2[0] = t._a->center();
    p2[1] = t._b->center();
    p2[2] = t._c->center();

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
bool ArtifactTriangle::isSimilarTo(const ArtifactTriangle &t,
                                   const double eps)
{
    double s1[3]; //массив сторон
    s1[0] = ac::calcDistance(this->_a->center(), this->_b->center());
    s1[1] = ac::calcDistance(this->_a->center(), this->_c->center());
    s1[2] = ac::calcDistance(this->_b->center(), this->_c->center());

    double s2[3]; //массив сторон
    s2[0] = ac::calcDistance(t._a->center(), t._b->center());
    s2[1] = ac::calcDistance(t._a->center(), t._c->center());
    s2[2] = ac::calcDistance(t._b->center(), t._c->center());

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
                goto  found;
        }
    }
    return false;

found:
    for(int k=0; k < 3; ++k)
    {
        if((k == i) || (k == j))
            continue;
        if(qAbs(r3[k] - r2[j]) < eps)
            return true;
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
