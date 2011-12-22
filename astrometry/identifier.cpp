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
bool distEqualTest(const ArtifactPair &p1,
                   const ArtifactPair &p2,
                   const double distEps)
{
    if(distEps <= 0)    return false;
    double diff = qAbs(p1.dist() - p2.dist());
    if(diff < distEps)
    {
        return true;
        qDebug() << "!!! equal distances:" << "\n"
                 << "       " << p1.star() << "\n"
                 << "       " << p2.star();
    }
    else
    {
        return false;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void cookPairVector(const ArtifactVector &av,
                    const Artifact &refStar,
                    const double distEps,
                    ArtifactPairVector &pv)
{
    pv.clear();
    ArtifactVector::const_iterator it = av.constBegin();
    for(; it < av.constEnd(); ++it)
    {
        ArtifactPair pair(&refStar, *it);
        if(pair.dist() > 0)
        {
            if(!pv.empty()) //удаление звёзд, имеющих одинаковое расстояние до опорной звезды
            {
                if(distEqualTest(pair, pv.back(), distEps))
                {
                    pv.pop_back();
                    continue;
                }
            }

            pv.push_back(pair);
        }
    }
    qSort(pv); //по возрастанию расстояния от опорной звезды
}
/////////////////////////////////////////////////////////////////////////////////////
void findRefStar(const ArtifactVector &v,
                 const QPointF        &picCenter,
                 Artifact             &refStar)
{
    ArtifactVector candidates; //звёзды одинаковой и максимальной яркости (кандидаты на опорную)
    ArtifactVector::const_iterator it = v.constBegin();
    for(; it < v.constEnd(); ++it)
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
        cookPairVector(candidates, cen, 0, pairs);
        refStar = pairs.first().star();
    }
    else
    {
        refStar = v.front();
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
                const double    similarEps,
                const double    equalEps,
                const QPointF  &screenCenter)
{
    if(picStars.size() < __minStarQuantity)     return;
    if(catStars.size() < __minStarQuantity)     return;

//    if(picStars.size() > catStars.size())
//    {
//        art::cutoff(picStars, catStars.size());
//    }
//    if(catStars.size() > picStars.size())
//    {
//        art::cutoff(catStars, picStars.size());
//    }

    Artifact picRefStar;
    Artifact catRefStar;
    findRefStar(picStars,
                screenCenter,
                picRefStar);
    findRefStar(catStars,
                screenCenter,
                catRefStar);

    if(!art::isEqual(picRefStar, catRefStar, equalEps))
    {
        qDebug() << "!!! ref stars is differ: " << "\n"
                 << "       pic: " << picRefStar << "\n"
                 << "       cat: " << catRefStar;
        return;
    }

    ArtifactPairVector picPairs, catPairs;
    cookPairVector(picStars, picRefStar, __distEps, picPairs);
    cookPairVector(catStars, catRefStar, __distEps, catPairs);

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

    qDebug() << "pairs quantity: " << ratioVec.size() << "\n"
             << "   ratio vec: " << ratioVec;

    QVector_int posVec; //массив с позициями отношений расстояний, равных опорному (первое)
                        //т.е. отождествляемые звёзды имеют те же позиции в ArtifactPairVector
    findEqualRatios(ratioVec, similarEps, posVec);

    qDebug() << "similar: " << posVec.size();

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
