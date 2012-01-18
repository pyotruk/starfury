#include "triangle.h"
/////////////////////////////////////////////////////////////////////////////////////
//проверка треугольников на равенство (совпадение вершин)
bool isEqual(const ArtifactTriangle &t1,
             const ArtifactTriangle &t2,
             const double eps)
{
    QPointF p1[3]; //массив точек
    p1[0] = t1.t()[0].center();
    p1[1] = t1.t()[1].center();
    p1[2] = t1.t()[2].center();

    QPointF p2[3]; //массив точек
    p2[0] = t2.t()[0].center();
    p2[1] = t2.t()[1].center();
    p2[2] = t2.t()[2].center();

    int eq = 0;

    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
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
//проверка треугольников на подобие
bool isSimilar(const ArtifactTriangle &t1,
               const ArtifactTriangle &t2,
               const double eps,
               SimPath& sim)
{
    double s1[3]; //массив сторон
    s1[0] = ac::calcDistance(t1.t()[1].center(), t1.t()[2].center());
    s1[1] = ac::calcDistance(t1.t()[0].center(), t1.t()[2].center());
    s1[2] = ac::calcDistance(t1.t()[0].center(), t1.t()[1].center());

    double s2[3]; //массив сторон
    s2[0] = ac::calcDistance(t2.t()[1].center(), t2.t()[2].center());
    s2[1] = ac::calcDistance(t2.t()[0].center(), t2.t()[2].center());
    s2[2] = ac::calcDistance(t2.t()[0].center(), t2.t()[1].center());

    double r1[3]; //массив отношений сторон
    r1[0] = s1[0] / s2[0];
    r1[1] = s1[0] / s2[1];
    r1[2] = s1[0] / s2[2];

    double r2[3]; //массив отношений сторон
    r2[0] = s1[1] / s2[0];
    r2[1] = s1[1] / s2[1];
    r2[2] = s1[1] / s2[2];

    double r3[3]; //массив отношений сторон
    r3[0] = s1[2] / s2[0];
    r3[1] = s1[2] / s2[1];
    r3[2] = s1[2] / s2[2];

    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            if(j == i)
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
    for(int k = 0; k < 3; ++k)
    {
        if((k == sim[0]) || (k == sim[1]))
            continue;
        if(qAbs(r3[k] - r2[sim[1]]) < eps)
        {
            sim[2] = k;
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
//формирует вектор треугольников из вектора артефактов
void cookTriangles(const ArtifactVector &a,
                   TriangleVector &t)
{
    t.clear();
    ComboVector comboVec;
    int i, j, k;
    i = 0;
    for(ArtifactVector::const_iterator
        it1 = a.constBegin(); it1 < a.constEnd(); ++it1, ++i)
    {
        j = 0;
        for(ArtifactVector::const_iterator
            it2 = a.constBegin(); it2 < a.constEnd(); ++it2, ++j)
        {
            if(it2 == it1)    continue;
            k = 0;
            for(ArtifactVector::const_iterator
                it3 = a.constBegin(); it3 < a.constEnd(); ++it3, ++k)
            {
                if((it3 == it1) || (it3 == it2))    continue;
                Combo c(i, j, k);
                if(!comb::isContain(comboVec, c))
                {
                    t.push_back(ArtifactTriangle(*it1, *it2, *it3));
                    comboVec.push_back(c);
                }
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
//удаляет повторяющиеся треугольники
void deleteEqual(TriangleVector &t,
                 const double eps)
{
    for(TriangleVector::iterator
        i = t.begin(); i < t.end(); ++i)
    {
        for(TriangleVector::iterator
            j = t.begin(); j < t.end(); ++j)
        {
            if(j == i)    continue;
            if(isEqual(*i, *j, eps))
            {
                t.erase(j);
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
//формирует вектора с отождествлёнными треугольниками
void findSimilarTriangles(TriangleVector &picTriangles,
                          TriangleVector &catTriangles,
                          const double eps)
{
    SimPath sim;
    TriangleVector tvPic;
    TriangleVector tvCat;
    for(TriangleVector::iterator
        itPic = picTriangles.begin(); itPic < picTriangles.end(); ++itPic)
    {
        for(TriangleVector::iterator
            itCat = catTriangles.begin(); itCat < catTriangles.end(); ++itCat)
        {
            if(isSimilar(*itPic, *itCat, eps, sim))
            {
                tvPic.push_back(*itPic);
                tvCat.push_back(ArtifactTriangle(itCat->t()[sim[0]],
                                                 itCat->t()[sim[1]],
                                                 itCat->t()[sim[2]]));
                break;
            }
        }
    }
    picTriangles = tvPic;
    catTriangles = tvCat;
}
/////////////////////////////////////////////////////////////////////////////////////
void extractArtifacts(const TriangleVector &tvPic,
                      const TriangleVector &tvCat,
                      const double equalEps,
                      ArtifactVector &picStars,
                      ArtifactVector &catStars)
{
    picStars.clear();
    catStars.clear();
    Artifact a;
    for(TriangleVector::const_iterator itPic = tvPic.constBegin(),
                                       itCat = tvCat.constBegin();
        itPic < tvPic.constEnd();
        ++itPic, ++itCat)
    {
        for(int i = 0; i < 3; ++i)
        {
            a = itPic->t()[i];
            if(!art::isContain(picStars, a, equalEps))
            {
                picStars.push_back(a);
            }
            a = itCat->t()[i];
            if(!art::isContain(catStars, a, equalEps))
            {
                catStars.push_back(a);
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
int simtri::equate(ArtifactVector &picStars,
                   ArtifactVector &catStars,
                   const double similarEps,
                   const int minEquatedStarQuantity)
{
    if(picStars.size() < astrometry::_minStarQuantity)     return astrometry::TOO_LESS_RAW_STARS;
    if(catStars.size() < astrometry::_minStarQuantity)     return astrometry::TOO_LESS_RAW_STARS;

    TriangleVector picTriangles;
    TriangleVector catTriangles;

    cookTriangles(picStars,
                  picTriangles);
    deleteEqual(picTriangles,
                _equalEps);

    cookTriangles(catStars,
                  catTriangles);
    deleteEqual(catTriangles,
                _equalEps);

    qDebug() << "triangles TOTAL:  " << picTriangles.size()
             << " (pic)    " << catTriangles.size() << " (cat)";

    findSimilarTriangles(picTriangles, catTriangles, similarEps);

    qDebug() << "triangles SIMILAR:  " << picTriangles.size()
             << " (pic)    " << catTriangles.size() << " (cat)";

    extractArtifacts(picTriangles,
                     catTriangles,
                     _equalEps,
                     picStars,
                     catStars);

    if(picStars.size() < minEquatedStarQuantity)    return astrometry::TOO_LESS_EQUATED_STARS;
    if(catStars.size() < minEquatedStarQuantity)    return astrometry::TOO_LESS_EQUATED_STARS;

    return astrometry::SUCCESS;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
