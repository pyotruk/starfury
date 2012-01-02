/*Класс для хранения параметров (центр масс, размер)
 объекта, найденного на изображении*/
#ifndef ARTIFACT_H
#define ARTIFACT_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QPointF>
#include <QVector>
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////
#include "math/astrocalc.h"
#include "boxes/databox.h"
/////////////////////////////////////////////////////////////////////////////////////
class Artifact
{
public:
    explicit Artifact();
    explicit Artifact(const double x,
                      const double y,
                      const double magn = 0);
    Artifact(const Artifact&);
    Artifact& operator =(const Artifact&);
    bool operator ==(const Artifact &a) const {return this->_magnitude == a._magnitude;}
    bool operator !=(const Artifact &a) const {return this->_magnitude != a._magnitude;}
    bool operator <(const Artifact &a)  const {return this->_magnitude <  a._magnitude;}
    const QPointF& center() const {return _center;}
    double magnitude() const {return _magnitude;}
    void setCenter(const QPointF &p) {_center = p;}
    void setMagnitude(const double m) {_magnitude = m;}
private:
    QPointF _center;
    double  _magnitude; //размер в усл.ед.
};
/////////////////////////////////////////////////////////////////////////////////////
QDebug operator<<(QDebug, const Artifact&);
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
typedef QVector<Artifact>       ArtifactVector;
typedef DataBox<ArtifactVector> ArtifactBox;
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
namespace art
{
void selectOnCircle(ArtifactVector&,       //удаление артефактов, которые не попали на круг
                    const QPointF &center,
                    const double radius);
void cutoff(ArtifactVector&,               //обрезка вектора до нужной длины
            const int newLength);               //если newLenght больше текущей длины, то ничего не происходит
bool isEqual(const Artifact &a1,           //проверка артефактов на равенство (совпадение центров масс)
             const Artifact &a2,
             const double eps);
bool isContain(const ArtifactVector &v,
               const Artifact &a,
               const double eps);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // ARTIFACT_H
