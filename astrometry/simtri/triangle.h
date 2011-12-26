#ifndef TRIANGLE_H
#define TRIANGLE_H
/////////////////////////////////////////////////////////////////////////////////////
/*  лассы дл€ решени€ задачи отождествлени€ звЄздных конфигураций
   методом подобных треугольников. */
/////////////////////////////////////////////////////////////////////////////////////
#include <QVector>
#include <QPointF>
#include <QtAlgorithms>
#include "qmath.h"
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/artifact.h"
#include "math/astrocalc.h"
#include "boxes/databox.h"
#include "astrometry/simtri/combo.h"
/////////////////////////////////////////////////////////////////////////////////////
typedef Artifact Triangle[3];
/////////////////////////////////////////////////////////////////////////////////////
class ArtifactTriangle
{
public:
    explicit ArtifactTriangle() {}
    explicit ArtifactTriangle(const Artifact &a,
                              const Artifact &b,
                              const Artifact &c)
    {_t[0] = a;  _t[1] = b;  _t[2] = c;}
    const Triangle& t() const {return _t;}
private:
    Triangle _t;
};
/////////////////////////////////////////////////////////////////////////////////////
typedef QVector<ArtifactTriangle> TriangleVector;
/////////////////////////////////////////////////////////////////////////////////////
struct TriangleBoxData
{
    TriangleVector picTriangles;
    TriangleVector catTriangles;
};
/////////////////////////////////////////////////////////////////////////////////////
typedef DataBox<TriangleBoxData> TriangleBox;
/////////////////////////////////////////////////////////////////////////////////////
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
void cookTriangles(const ArtifactVector&,        //формирует вектор треугольников из вектора артефактов
                   TriangleVector&);
void cookTriangleBox(TriangleBoxData&,     //формирует вектора с отождествлЄнными треугольниками
                     const double eps);
void triangles2Artifacts(const TriangleVector &tvPic,
                         const TriangleVector &tvCat,
                         const double equalEps,
                         ArtifactVector &picStars,
                         ArtifactVector &catStars);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
#endif // ARTTRIANGLE_H
