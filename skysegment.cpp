#include "skysegment.h"
/////////////////////////////////////////////////////////////////////////////////////
SkySegment::SkySegment(const double fieldWidth,
                       const double fieldHeight,
                       const double side,
                       const double edge) :
    _field(fieldWidth, fieldHeight),
    _side(side),
    _edge(edge)
{}
/////////////////////////////////////////////////////////////////////////////////////
bool SkySegment::isBelongToSkyRect(const double alpha,
                                   const double delta,
                                   SkyRect &rect)
{
    if((alpha >= rect.botLeft.alpha) && (alpha < rect.topRight.alpha))
    {
        if((delta >= rect.botLeft.delta ) && (delta < rect.topRight.delta))
        {
            return true;
        }
    }
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
void SkySegment::generateNew(const double alpha,
                             const double delta)
{
    double alphaSide = _field.width() * _side / qAbs(qCos(delta));
    double deltaSide = _field.height() * _side;
    _rect.topRight.alpha = alpha + alphaSide / 2;
    _rect.topRight.delta = delta + deltaSide / 2;
    _rect.botLeft.alpha = _rect.topRight.alpha - alphaSide;
    _rect.botLeft.delta = _rect.topRight.delta - deltaSide;
}
/////////////////////////////////////////////////////////////////////////////////////
void SkySegment::generateNew(const double alpha,
                             const double delta,
                             const double fieldWidth,
                             const double fieldHeight)
{
    this->setField(QSizeF(fieldWidth, fieldHeight));
    this->generateNew(alpha, delta);
}
/////////////////////////////////////////////////////////////////////////////////////
bool SkySegment::isBelong(const double alpha,
                          const double delta)
{
    return this->isBelongToSkyRect(alpha, delta, _rect);
}
/////////////////////////////////////////////////////////////////////////////////////
bool SkySegment::isOnEdge(const double alpha,
                          const double delta)
{
    SkyRect smallRect ;
    double edgeWidth, edgeHeight;
    edgeWidth  = _field.width() * _edge / qAbs(qCos(delta));
    edgeHeight = _field.height() * _edge;
    smallRect.botLeft.alpha = _rect.botLeft.alpha + edgeWidth;
    smallRect.botLeft.delta = _rect.botLeft.delta + edgeHeight;
    smallRect.topRight.alpha = _rect.topRight.alpha - edgeWidth;
    smallRect.topRight.delta = _rect.topRight.delta - edgeHeight;
    return !this->isBelongToSkyRect( alpha, delta, smallRect );
}
/////////////////////////////////////////////////////////////////////////////////////
void SkySegment::setField(const QSizeF &field)
{
    _field = field;
}
/////////////////////////////////////////////////////////////////////////////////////
void SkySegment::setSide(const double side)
{
    _side = side;
}
/////////////////////////////////////////////////////////////////////////////////////
void SkySegment::setEdge(const double edge)
{
    _edge = edge;
}
/////////////////////////////////////////////////////////////////////////////////////
QSizeF& SkySegment::field()
{
    return _field;
}
/////////////////////////////////////////////////////////////////////////////////////
double SkySegment::side()
{
    return _side;
}
/////////////////////////////////////////////////////////////////////////////////////
double SkySegment::edge()
{
    return _edge;
}
/////////////////////////////////////////////////////////////////////////////////////
SkyRect& SkySegment::rect()
{
    return _rect;
}
/////////////////////////////////////////////////////////////////////////////////////
