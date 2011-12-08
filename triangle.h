#ifndef TRIANGLE_H
#define TRIANGLE_H
/////////////////////////////////////////////////////////////////////////////////////
/*  лассы дл€ решени€ задачи отождествлени€ звЄздных конфигураций
   методом подобных треугольников. */
/////////////////////////////////////////////////////////////////////////////////////
#include <QVector>
#include <QPoint>
/////////////////////////////////////////////////////////////////////////////////////
#include "artifact.h"
#include "astrocalc.h"
#include "qmath.h"
/////////////////////////////////////////////////////////////////////////////////////
typedef Artifact* Triangle[3];
/////////////////////////////////////////////////////////////////////////////////////
class ArtifactTriangle
{
public:
    ArtifactTriangle(Artifact *a = 0,
                     Artifact *b = 0,
                     Artifact *c = 0)
    {_t[0] = a;  _t[1] = b;  _t[2] = c;}
    const Triangle& t() const {return _t;}
private:
    Triangle _t;
};
/////////////////////////////////////////////////////////////////////////////////////
typedef QVector<ArtifactTriangle> TriangleVector;
/////////////////////////////////////////////////////////////////////////////////////
typedef int SimPath[3]; //путь подоби€ (соответствие сторон треугольников)
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
namespace tri
{
bool isEqual(const ArtifactTriangle &t1,   //проверка треугольников на равенство (совпадение вершин)
             const ArtifactTriangle &t2,
             const double eps);
bool isSimilar(const ArtifactTriangle &t1, //проверка треугольников на подобие
               const ArtifactTriangle &t2,
               const double eps,
               SimPath&);
void deleteEqual(TriangleVector&,          //удал€ет повтор€ющиес€ треугольники
                 const double eps);
void cookTriangles(ArtifactVector&,        //формирует вектор треугольников из вектора артефактов
                   TriangleVector&);
void cookEquatedArtifacts(const TriangleVector &t1, //формирует вектора a1 и a2 с отождествлЄнными точками (артефактами)
                          const TriangleVector &t2,
                          const double eps,
                          ArtifactVector &a1,
                          ArtifactVector &a2);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
#endif // ARTTRIANGLE_H
