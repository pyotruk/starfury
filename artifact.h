/*Класс для хранения параметров (центр масс, размер)
 объекта, найденного на изображении*/
#ifndef ARTIFACT_H
#define ARTIFACT_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QPoint>
#include <QVector>
/////////////////////////////////////////////////////////////////////////////////////
#include "astrocalc.h"
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
    double _magnitude; //размер в усл.ед.
};
/////////////////////////////////////////////////////////////////////////////////////
typedef QVector<Artifact> ArtifactVector;
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
namespace art
{
void selectOnCircle(ArtifactVector&,       //удаление артефактов, которые не попали на круг
                    const QPoint &center,
                    const int radius);
void cutoff(ArtifactVector&,               //обрезка вектора до нужной длины
            const int newLength);               //если newLenght больше текущей длины, то ничего не происходит
bool isEqual(const Artifact &a1,           //проверка артефактов на равенство (совпадение центров масс)
             const Artifact &a2,
             const double eps);
void deleteEqual(ArtifactVector&,
                 const double eps);         //удаляет повторяющиеся артефакты
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // ARTIFACT_H
