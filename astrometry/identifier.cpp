#include "identifier.h"
/////////////////////////////////////////////////////////////////////////////////////
StarPair::StarPair() :
    _dist(0)
{}
/////////////////////////////////////////////////////////////////////////////////////
StarPair::StarPair(const Artifact &refStar,
                   const Artifact &anyStar)
{
    _ref = refStar;
    _any = anyStar;
    _dist = ac::calcDistance(_ref.center(), _any.center());
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void cookDistVector(const ArtifactVector &av,
                    DistVector &dv)
{
    dv.clear();
    ArtifactVector::const_iterator it = av.constBegin();
    Artifact refStar = *it;
    ++it;
    for(; it < av.constEnd(); ++it)
    {
        dv.push_back(StarPair(refStar, *it));
    }
    qSort(dv); //по возрастанию
}
/////////////////////////////////////////////////////////////////////////////////////
void id::equate(const ArtifactVector &picStars,
                const ArtifactVector &catStars,
                const double eps,
                ArtifactVector &eqPicStars,
                ArtifactVector &eqCatStars)
{
    if(picStars.size() < 2)                  return;
    if(picStars.size() > catStars.size())    return;
    DistVector picDist, catDist;
    cookDistVector(picStars, picDist);
    cookDistVector(catStars, catDist);

    QVector_double ratioVec; //массив отношений расстояний
    DistVector::const_iterator itPic = picDist.constBegin();
    DistVector::const_iterator itCat = catDist.constBegin();
    for(; itPic < picDist.constEnd(); ++itPic, ++itCat)
    {
        ratioVec.push_back(itPic->dist() / itCat->dist());
    }

    QVector_int posVec;
    QVector_double::const_iterator it = ratioVec.constBegin();
    double refRatio = *it;
    for(int i = 0; it < ratioVec.constEnd(); ++it, ++i)
    {
        if(qAbs(refRatio - *it) < eps)
        {
            posVec.push_back(i);
        }
    }
    if(posVec.empty())    return;

    eqPicStars.clear();
    eqCatStars.clear();
    /*DistVector::const_iterator*/ itPic = picDist.constBegin();
    /*DistVector::const_iterator*/ itCat = catDist.constBegin();
    eqPicStars.push_back(itPic->refStar());
    eqCatStars.push_back(itCat->refStar());
    for(int i = 0; itPic < picDist.constEnd(); ++itPic, ++itCat, ++i)
    {
        if(posVec.contains(i))
        {
            eqPicStars.push_back(itPic->anyStar());
            eqCatStars.push_back(itCat->anyStar());
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
