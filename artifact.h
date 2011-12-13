/*����� ��� �������� ���������� (����� ����, ������)
 �������, ���������� �� �����������*/
#ifndef ARTIFACT_H
#define ARTIFACT_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QPoint>
#include <QVector>
/////////////////////////////////////////////////////////////////////////////////////
#include "astrocalc.h"
#include "databox.h"
/////////////////////////////////////////////////////////////////////////////////////
class Artifact
{
public:
    Artifact();
    Artifact(const Artifact&);
    Artifact& operator =(const Artifact&);
    bool operator ==(const Artifact &a) const {return this->_magnitude == a._magnitude;}
    bool operator !=(const Artifact &a) const {return this->_magnitude != a._magnitude;}
    bool operator <(const Artifact &a)  const {return this->_magnitude <  a._magnitude;}
    const QPoint& center() const {return _center;}
    double magnitude() const {return _magnitude;}
    void setCenter(const QPoint&);
    void setMagnitude(const double);
private:
    QPoint _center;
    double _magnitude; //������ � ���.��.
};
/////////////////////////////////////////////////////////////////////////////////////
typedef QVector<Artifact>       ArtifactVector;
typedef DataBox<ArtifactVector> ArtifactBox;
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
namespace art
{
void selectOnCircle(ArtifactVector&,       //�������� ����������, ������� �� ������ �� ����
                    const QPoint &center,
                    const int radius);
void cutoff(ArtifactVector&,               //������� ������� �� ������ �����
            const int newLength);               //���� newLenght ������ ������� �����, �� ������ �� ����������
bool isEqual(const Artifact &a1,           //�������� ���������� �� ��������� (���������� ������� ����)
             const Artifact &a2,
             const double eps);
void deleteEqual(ArtifactVector&,
                 const double eps);         //������� ������������� ���������
bool isContain(const ArtifactVector&,
               const Artifact&,
               const double eps);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // ARTIFACT_H
