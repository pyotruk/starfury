#include "identifier.h"
/////////////////////////////////////////////////////////////////////////////////////
ArtifactPair::ArtifactPair() :
    _dist(0)
{}
/////////////////////////////////////////////////////////////////////////////////////
ArtifactPair::ArtifactPair(const Artifact *refStar,
                           const Artifact &anyStar)
{
    _refStar = refStar;
    _star = anyStar;
    _dist = ac::calcDistance(_refStar->center(), _star.center());
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void cookPairVector(const ArtifactVector &av,
                    const Artifact &refStar,
                    ArtifactPairVector &pv)
{
    pv.clear();
    ArtifactVector::const_iterator it = av.constBegin();
    for(; it < av.constEnd(); ++it)
    {
        pv.push_back(ArtifactPair(&refStar, *it));
    }
    qSort(pv); //по возрастанию расстояния от опорной звезды
}
/////////////////////////////////////////////////////////////////////////////////////
void extractRefStar(ArtifactVector &v,
                    const QPointF  &picCenter,
                    Artifact       &refStar)
{
    ArtifactVector candidates; //звёзды одинаковой и максимальной яркости (кандидаты на опорную)
    ArtifactVector::iterator it = v.begin();
    for(; it < v.end(); ++it)
    {
        candidates.push_back(*it);
        if(*it != *(it + 1))
        {
            break;
        }
    }

    if(candidates.size() > 1)
    {
        Artifact cen(picCenter.x(), picCenter.y()); //гипотетическая звезда, находящаяся в центре кадра
        ArtifactPairVector pairs;
        cookPairVector(v, cen, pairs);
        refStar = pairs.first().star();
        it = qBinaryFind(v.begin(), v.begin() + candidates.size(), refStar);
        v.erase(it);
    }
    else
    {
        refStar = v.first();
        v.pop_front();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void cutoff(ArtifactPairVector &a,     //обрезка вектора до нужной длины
            const int newLength)            //если newLenght больше текущей длины, то ничего не происходит
{
    int dif = a.size() - newLength;
    if(dif > 0)     a.erase(a.end() - dif, a.end());
}
/////////////////////////////////////////////////////////////////////////////////////
void cookRatioVector(const ArtifactPairVector &picPairs,
                     const ArtifactPairVector &catPairs,
                     QVector_double &ratioVec)
{
    ArtifactPairVector::const_iterator itPic = picPairs.constBegin();
    ArtifactPairVector::const_iterator itCat = catPairs.constBegin();
    for(; itPic < picPairs.constEnd(); ++itPic, ++itCat)
    {
        ratioVec.push_back(itPic->dist() / itCat->dist());
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void findEqualRatios(const QVector_double &ratioVec,
                     const double eps,
                     QVector_int &posVec)
{
    QVector_double::const_iterator it = ratioVec.constBegin();
    double refRatio = *it;
    for(int i = 0; it < ratioVec.constEnd(); ++it, ++i)
    {
        if(qAbs(refRatio - *it) < eps)
        {
            posVec.push_back(i);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void extractSimilarStars(const ArtifactPairVector &picPairs,
                         const ArtifactPairVector &catPairs,
                         const Artifact           &picRefStar,
                         const Artifact           &catRefStar,
                         const QVector_int        &posVec,
                         ArtifactVector &eqPicStars,
                         ArtifactVector &eqCatStars)
{
    eqPicStars.clear();
    eqCatStars.clear();
    eqPicStars.push_back(picRefStar);
    eqCatStars.push_back(catRefStar);
    ArtifactPairVector::const_iterator itPic = picPairs.constBegin();
    ArtifactPairVector::const_iterator itCat = catPairs.constBegin();
    for(int i = 0; itPic < picPairs.constEnd(); ++itPic, ++itCat, ++i)
    {
        if(posVec.contains(i))
        {
            eqPicStars.push_back(itPic->star());
            eqCatStars.push_back(itCat->star());
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void id::equate(ArtifactVector &picStars,
                ArtifactVector &catStars,
                const double eps,
                const QPointF &screenCenter)
{
    if(picStars.size() < __minStarQuantity)     return;
    if(catStars.size() < __minStarQuantity)     return;

    Artifact picRefStar;
    Artifact catRefStar;
    extractRefStar(picStars,
                   screenCenter,
                   picRefStar);
    extractRefStar(catStars,
                   screenCenter,
                   catRefStar);

    ArtifactPairVector picPairs, catPairs;
    cookPairVector(picStars, picRefStar, picPairs);
    cookPairVector(catStars, catRefStar, catPairs);
    if(picPairs.size() > catPairs.size())
    {
        cutoff(picPairs, catPairs.size());
    }
    if(catPairs.size() > picPairs.size())
    {
        cutoff(catPairs, picPairs.size());
    }

    QVector_double ratioVec; //массив отношений расстояний
    cookRatioVector(picPairs, catPairs, ratioVec);

    QVector_int posVec; //массив с позициями отношений расстояний, равных опорному (первое)
                        //т.е. отождествляемые звёзды имеют те же позиции
    findEqualRatios(ratioVec, eps, posVec);
    if(posVec.empty())    return;

    extractSimilarStars(picPairs, catPairs,
                        picRefStar, catRefStar,
                        posVec,
                        picStars, catStars);
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
