#ifndef STAR_H
#define STAR_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QVector>
/////////////////////////////////////////////////////////////////////////////////////
class Star
{
public:
    explicit Star();
    explicit Star(const double alpha,
                  const double delta,
                  const double magn);
    bool operator ==(const Star &s) const {return this->_magnitude == s._magnitude;}
    bool operator !=(const Star &s) const {return this->_magnitude != s._magnitude;}
    bool operator <(const Star &s)  const {return this->_magnitude <  s._magnitude;}
    void setAlpha(const double);
    void setDelta(const double);
    void setMagnitude(const double);
    const double alpha(); //rad
    const double delta(); //rad
    const double magnitude();
private:
    double _alpha;
    double _delta;
    double _magnitude;
};
/////////////////////////////////////////////////////////////////////////////////////
typedef QVector<Star> StarVector;
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STAR_H
