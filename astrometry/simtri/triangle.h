#ifndef TRIANGLE_H
#define TRIANGLE_H
/////////////////////////////////////////////////////////////////////////////////////
/* ������ ��� ������� ������ �������������� ������� ������������
   ������� �������� �������������. */
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
typedef int SimPath[3]; //���� ������� (������������ ������ �������������)
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
namespace tri
{
bool isEqual(const ArtifactTriangle &t1,   //�������� ������������� �� ��������� (���������� ������)
             const ArtifactTriangle &t2,
             const double eps);
bool isSimilar(const ArtifactTriangle &t1, //�������� ������������� �� �������
               const ArtifactTriangle &t2,
               const double eps,
               SimPath&);
void deleteEqual(TriangleVector&,          //������� ������������� ������������
                 const double eps);
void cookTriangles(const ArtifactVector&,        //��������� ������ ������������� �� ������� ����������
                   TriangleVector&);
void cookTriangleBox(TriangleBoxData&,     //��������� ������� � ��������������� ��������������
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
