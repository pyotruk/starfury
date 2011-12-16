#ifndef SKYSEGMENT_H
#define SKYSEGMENT_H
/////////////////////////////////////////////////////////////////////////////////////
#include <qmath.h>
#include <QtGlobal>
#include <QSizeF>
/////////////////////////////////////////////////////////////////////////////////////
struct SkyPoint
{
    double alpha;
    double delta;
};
/////////////////////////////////////////////////////////////////////////////////////
struct SkyRect
{
    SkyPoint topRight;
    SkyPoint botLeft;
};
/////////////////////////////////////////////////////////////////////////////////////
class SkySegment
{
public:
    explicit SkySegment(const double fieldWidth = _defaultFieldWidth,
                        const double fieldHeight = _defaultFieldHeight,
                        const double side = _defaultSide, //������� �������� � �������� ������� ���� ������
                        const double edge = _defaultEdge); //������� ���� �������� � �������� ������� ����
    void generateNew(const double alpha,
                     const double delta,
                     const double fieldWidth,
                     const double fieldHeight);
    void generateNew(const double alpha,
                     const double delta);
    bool isBelong(const double alpha,
                  const double delta);
    bool isOnEdge(const double alpha,
                  const double delta);
    void setField(const QSizeF&);
    void setSide(const double);
    void setEdge(const double);
    const QSizeF& field() const {return _field;}
    double side() const {return _side;}
    double edge() const {return _edge;}
    const SkyRect& rect() const {return _rect;}
private:
    static const double _defaultFieldWidth  = 0.1;
    static const double _defaultFieldHeight = 0.1;
    static const double _defaultSide = 4.0;
    static const double _defaultEdge = 0.0;
    QSizeF     _field; //���� ������ [���]
    double     _side;  //������� �������� � �������� ������� ���� ������
    double     _edge;  //������� ���� �������� � �������� ������� ����
    SkyRect    _rect;
    bool isBelongToSkyRect(const double alpha,
                           const double delta,
                           SkyRect&);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // SKYSEGMENT_H
