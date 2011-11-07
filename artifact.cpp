#include "artifact.h"
/////////////////////////////////////////////////////////////////////////////////////
Artifact::Artifact() :
    _center(0, 0),
    _magnitude(0)
{}
/////////////////////////////////////////////////////////////////////////////////////
Artifact::Artifact(const Artifact &a)
{
    this->_center = a._center;
    this->_magnitude = a._magnitude;
}
/////////////////////////////////////////////////////////////////////////////////////
Artifact& Artifact::operator =(const Artifact &a)
{
    if(this != &a)
    {
        this->_center = a._center;
        this->_magnitude = a._magnitude;
    }
    return *this;
}
/////////////////////////////////////////////////////////////////////////////////////
QPoint& Artifact::center()
{
    return _center;
}
/////////////////////////////////////////////////////////////////////////////////////
double& Artifact::magnitude()
{
    return _magnitude;
}
/////////////////////////////////////////////////////////////////////////////////////
void Artifact::setCenter(QPoint &p)
{
    _center = p;
}
/////////////////////////////////////////////////////////////////////////////////////
void Artifact::setMagnitude(double &m)
{
    _magnitude = m;
}
/////////////////////////////////////////////////////////////////////////////////////
