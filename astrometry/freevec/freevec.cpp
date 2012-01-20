#include "freevec.h"
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
                   const double distDiffEps)
{
    if(distDiffEps <= 0)    return false;
    double diff = qAbs(p1.dist() - p2.dist());
    if(diff < distDiffEps)    return true;
    else    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
void cookPairVector(const ArtifactVector &av,
                    const Artifact &refStar,
                    const double distDiffEps,
                    ArtifactPairVector &pv)
{
    pv.clear();
    ArtifactVector::const_iterator it = av.constBegin();
    for(; it < av.constEnd(); ++it)
    {
        ArtifactPair pair(&refStar, *it);
        if(pair.dist() > 0)
        {
            if(!pv.empty()) //�������� ����, ������� ���������� ���������� �� ������� ������
            {
                if(distEqualTest(pair, pv.back(), distDiffEps))
                {
                    pv.pop_back();
                    continue;
                }
            }

            pv.push_back(pair);
        }
    }
    qSort(pv); //�� ����������� ���������� �� ������� ������
}
/////////////////////////////////////////////////////////////////////////////////////
void findRefStar(ArtifactVector &v,
                 const QPointF  &picCenter,
                 Artifact       &refStar)
{
    //����� ���� ���������� � ������������ ������� (��������� �� �������)
    ArtifactVector candidates;
    for(ArtifactVector::const_iterator
        it = v.constBegin(); it < v.constEnd(); ++it)
    {
        candidates.push_back(*it);
        if(*it != *(it + 1))
        {
            break;
        }
    }

    //����� ��������� � ������ ������ �� ����������
    if(candidates.size() > 1)
    {
        Artifact cen(picCenter.x(), picCenter.y()); //�������������� ������, ����������� � ������ �����
        ArtifactPairVector pairs;
        cookPairVector(candidates, cen, 0, pairs);
        refStar = pairs.first().star();
    }
    else
    {
        refStar = v.front();
    }

    //�������� ������� ������ �� ��������� �������
    ArtifactVector::iterator it = v.begin();
    while(it < v.end())
    {
        double dist = ac::calcDistance(refStar.center(), it->center());
        if(dist <= 0)
        {
            v.erase(it);
            break;
        }
        else
        {
            ++it;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void cutoff(ArtifactPairVector &a,     //������� ������� �� ������ �����
            const int newLength)            //���� newLenght ������ ������� �����, �� ������ �� ����������
{
    if(a.empty())    return;
    int dif = a.size() - newLength;
    if(dif > 0)     a.erase(a.end() - dif, a.end());
}
/////////////////////////////////////////////////////////////////////////////////////
void findSimilarStars(ArtifactPairVector &picPairs,
                      ArtifactPairVector &catPairs,
                      const Artifact   &picRefStar0,
                      const Artifact   &picRefStar1,
                      const Artifact   &catRefStar0,
                      const Artifact   &catRefStar1,
                      const double     eps,
                      ArtifactVector     &eqPicStars,
                      ArtifactVector     &eqCatStars)
{
    double refRatio = ac::calcDistance(picRefStar0.center(), picRefStar1.center()) /
                      ac::calcDistance(catRefStar0.center(), catRefStar1.center());
    qDebug() << "freevec: findSimilarStars(): ref ratio = " << refRatio;

    eqPicStars.clear();
    eqCatStars.clear();
    eqPicStars.push_back(picRefStar0);
    eqPicStars.push_back(picRefStar1);
    eqCatStars.push_back(catRefStar0);
    eqCatStars.push_back(catRefStar1);

    ArtifactPairVector::iterator itPic = picPairs.begin();
    ArtifactPairVector::iterator itCat = catPairs.begin();

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
                //qDebug() << "freevec: findSimilarStars(): similar found, ratio = " << ratio;
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
int freevec::equate(ArtifactVector &picStars,
                    ArtifactVector &catStars,
                    const QSize    &screen,
                    const double    similarEps,
                    const int       minEquatedStarQuantity)
{
    if(picStars.size() < astrometry::_minStarQuantity)     return astrometry::TOO_LESS_RAW_STARS;
    if(catStars.size() < astrometry::_minStarQuantity)     return astrometry::TOO_LESS_RAW_STARS;

    QPointF screenCenter(screen.width() / 2, screen.height() / 2);
    Artifact picRefStar0, picRefStar1;
    Artifact catRefStar0, catRefStar1;
    findRefStar(picStars,
                screenCenter,
                picRefStar0);
    findRefStar(picStars,
                screenCenter,
                picRefStar1);
    findRefStar(catStars,
                screenCenter,
                catRefStar0);
    findRefStar(catStars,
                screenCenter,
                catRefStar1);

    ArtifactPairVector picPairs, catPairs;
    cookPairVector(picStars, picRefStar0, _distDiffEps, picPairs);
    cookPairVector(catStars, catRefStar0, _distDiffEps, catPairs);

    findSimilarStars(picPairs,
                     catPairs,
                     picRefStar0,
                     picRefStar1,
                     catRefStar0,
                     catRefStar1,
                     similarEps,
                     picStars,
                     catStars);

    qDebug() << "freevec: equate(): " << picStars.size() << " similar stars found";

    if(picStars.size() < minEquatedStarQuantity)    return astrometry::TOO_LESS_EQUATED_STARS;
    if(catStars.size() < minEquatedStarQuantity)    return astrometry::TOO_LESS_EQUATED_STARS;

    return astrometry::SUCCESS;
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
