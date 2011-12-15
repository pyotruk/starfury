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
    void setAlpha(const double alpha) {_alpha = alpha;}
    void setDelta(const double delta) {_delta = delta;}
    void setMagnitude(const double magn) {_magnitude = magn;}
    const double alpha() const {return _alpha;} //rad
    const double delta() const {return _delta;} //rad
    const double magnitude() const {return _magnitude;}
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
