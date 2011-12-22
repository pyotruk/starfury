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
/////////////////////////////////////////////////////////////////////////////////////
void deleteNearStars(ArtifactVector &a,
                     const double eps)
{
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
void findSimilarStars(ArtifactPairVector &picPairs,
                      ArtifactPairVector &catPairs,
                      const Artifact   &picRefStar,
                      const Artifact   &catRefStar,
                      const double     eps,
                      ArtifactVector     &eqPicStars,
                      ArtifactVector     &eqCatStars)
{
    eqPicStars.clear();
    eqCatStars.clear();
    eqPicStars.push_back(picRefStar);
    eqCatStars.push_back(catRefStar);
    ArtifactPairVector::iterator itPic = picPairs.begin();
    ArtifactPairVector::iterator itCat = catPairs.begin();
    double refRatio = itPic->dist() / itCat->dist();
    eqPicStars.push_back(itPic->star());
    eqCatStars.push_back(itCat->star());
    picPairs.pop_front();
    catPairs.pop_front();

    itPic = picPairs.begin();
    while(itPic < picPairs.end())
    {
        itCat = catPairs.begin();
        bool found = false;
        while(itCat < catPairs.end())
        {
            double ratio = itPic->dist() / itCat->dist();
            double diff = qAbs(refRatio - ratio);
            if(diff < eps)
            {
                qDebug() << "SIMILAR: " << "diff = " << diff << "\n"
                         << "   ref ratio: " << refRatio << "    ratio: " << ratio << "\n"
                         << "       pic: " << itPic->star() << "\n"
                         << "       cat: " << itCat->star() << "\n";
                eqPicStars.push_back(itPic->star());
                eqCatStars.push_back(itCat->star());
                itPic = picPairs.erase(itPic);
                itCat = catPairs.erase(itCat);
                found = true;
                break;
            }
            else
            {
                ++itCat;
            }
        }
        if(!found)
        {
            ++itPic;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void id::equate(ArtifactVector &picStars,
                ArtifactVector &catStars,
                const double similarEps,
                const QPointF &screenCenter)
{
    if(picStars.size() < __minStarQuantity)     return;
    if(catStars.size() < __minStarQuantity)     return;

    deleteNearStars(picStars, __maxNearStarDist);
    deleteNearStars(catStars, __maxNearStarDist);

    if(picStars.size() > catStars.size())
    {
        art::cutoff(picStars, catStars.size());
    }
    if(catStars.size() > picStars.size())
    {
        art::cutoff(catStars, picStars.size());
    }

    Artifact picRefStar;
    Artifact catRefStar;
    findRefStar(picStars,
                screenCenter,
                picRefStar);
    findRefStar(catStars,
                screenCenter,
                catRefStar);

    ArtifactPairVector picPairs, catPairs;
    cookPairVector(picStars, picRefStar, __distEps, picPairs);
    cookPairVector(catStars, catRefStar, __distEps, catPairs);

    findSimilarStars(picPairs,
                     catPairs,
                     picRefStar,
                     catRefStar,
                     similarEps,
                     picStars,
                     catStars);
    qDebug() << "FOUND  _" << picStars.size() << "_ similar stars";
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
