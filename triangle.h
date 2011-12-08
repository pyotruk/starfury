#ifndef TRIANGLE_H
#define TRIANGLE_H
/////////////////////////////////////////////////////////////////////////////////////
/* ������ ��� ������� ������ �������������� ������� ������������
   ������� �������� �������������. */
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
void cookTriangles(ArtifactVector&,        //��������� ������ ������������� �� ������� ����������
                   TriangleVector&);
void cookEquatedArtifacts(const TriangleVector &t1, //��������� ������� a1 � a2 � ��������������� ������� (�����������)
                          const TriangleVector &t2,
                          const double eps,
                          ArtifactVector &a1,
                          ArtifactVector &a2);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
#endif // ARTTRIANGLE_H