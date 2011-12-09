#include "triangle.h"
/////////////////////////////////////////////////////////////////////////////////////
bool tri::isEqual(const ArtifactTriangle &t1,
                  const ArtifactTriangle &t2,
                  const double eps)
{
    QPoint p1[3]; //������ �����
    p1[0] = t1.t()[0]->center();
    p1[1] = t1.t()[1]->center();
    p1[2] = t1.t()[2]->center();

    QPoint p2[3]; //������ �����
    p2[0] = t2.t()[0]->center();
    p2[1] = t2.t()[1]->center();
    p2[2] = t2.t()[2]->center();

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
bool tri::isSimilar(const ArtifactTriangle &t1,
                    const ArtifactTriangle &t2,
                    const double eps,
                    SimPath& sim)
{
    double s1[3]; //������ ������
    s1[0] = ac::calcDistance(t1.t()[1]->center(), t1.t()[2]->center());
    s1[1] = ac::calcDistance(t1.t()[0]->center(), t1.t()[2]->center());
    s1[2] = ac::calcDistance(t1.t()[0]->center(), t1.t()[1]->center());

    double s2[3]; //������ ������
    s2[0] = ac::calcDistance(t2.t()[1]->center(), t2.t()[2]->center());
    s2[1] = ac::calcDistance(t2.t()[0]->center(), t2.t()[2]->center());
    s2[2] = ac::calcDistance(t2.t()[0]->center(), t2.t()[1]->center());

    double r1[3]; //������ ��������� ������
    r1[0] = s1[0] / s2[0];
    r1[1] = s1[0] / s2[1];
    r1[2] = s1[0] / s2[2];

    double r2[3]; //������ ��������� ������
    r2[0] = s1[1] / s2[0];
    r2[1] = s1[1] / s2[1];
    r2[2] = s1[1] / s2[2];

    double r3[3]; //������ ��������� ������
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
void tri::cookTriangles(ArtifactVector &a,
                        TriangleVector &t)
{
    t.clear();
    for(ArtifactVector::iterator
        i = a.begin(); i < a.end(); ++i)
    {
        for(ArtifactVector::iterator
            j = a.begin(); j < a.end(); ++j)
        {
            if(j == i)    continue;
            for(ArtifactVector::iterator
                k = a.begin(); k < a.end(); ++k)
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
    for(TriangleVector::iterator
        i = t.begin(); i < t.end(); ++i)
    {
        for(TriangleVector::iterator
            j = t.begin(); j < t.end(); ++j)
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
void tri::cookTriangleBox(TriangleBoxData &d,
                          const double eps)
{
    SimPath sim;
    ArtifactVector aPic;
    TriangleVector tPic;
    ArtifactVector aCat;
    TriangleVector tCat;
    Triangle tp, tc;
    Artifact a;
    ArtifactVector::iterator it;
    for(TriangleVector::iterator
        itPic = d.picTriangles.begin(); itPic < d.picTriangles.end(); ++itPic)
    {
        for(TriangleVector::iterator
            itCat = d.catTriangles.begin(); itCat < d.catTriangles.end(); ++itCat)
        {
            if(tri::isSimilar(*itPic, *itCat, eps, sim))
            {
                for(int i = 0; i < 3; ++i)
                {
                    a = *(itPic->t()[i]);
                    it = qBinaryFind(aPic.begin(), aPic.end(), a);
                    if(it < aPic.end())
                    {
                        tp[i] = it;
                    }
                    else
                    {
                        aPic.push_back(a);
                        tp[i] = &(aPic.back());
                    }

                    a = *(itCat->t()[sim[i]]);
                    it = qBinaryFind(aCat.begin(), aCat.end(), a);
                    if(it < aCat.end())
                    {
                        tc[i] = it;
                    }
                    else
                    {
                        aCat.push_back(a);
                        tc[i] = &(aCat.back());
                    }
                }
                tPic.push_back(ArtifactTriangle(tp[0], tp[1], tp[2]));
                tCat.push_back(ArtifactTriangle(tc[0], tc[1], tc[2]));
                break;
            }
        }
    }
    d.picStars = aPic;
    d.picTriangles = tPic;
    d.catStars = aCat;
    d.catTriangles = tCat;
}
/////////////////////////////////////////////////////////////////////////////////////
