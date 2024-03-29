#include "drawing.h"
/////////////////////////////////////////////////////////////////////////////////////
void draw::crossbuck(QImage &img,
                     const QPointF &center,
                     const int side,
                     const QColor &color,
                     const int pen)
{
    const int side2 = side / 2;
    QPainter painter(&img);
    painter.setPen(QPen(color, pen));
    painter.drawLine(QPointF(center.x() - side2, center.y() - side2),
                     QPointF(center.x() + side2, center.y() + side2));
    painter.drawLine(QPointF(center.x() + side2, center.y() - side2),
                     QPointF(center.x() - side2, center.y() + side2));
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::cross(QImage &img,
                 const QPointF &center,
                 const int side,
                 const QColor &color,
                 const int pen)
{
    const double side2 = side / 2;
    QPainter painter(&img);
    painter.setPen(QPen(color, pen));
    painter.drawLine(QPointF(center.x(), center.y() - side2),
                     QPointF(center.x(), center.y() + side2));
    painter.drawLine(QPointF(center.x() - side2, center.y()),
                     QPointF(center.x() + side2, center.y()));
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::triangle(QImage &img,
                    const QPointF &p1,
                    const QPointF &p2,
                    const QPointF &p3,
                    const QColor &color,
                    const int pen)
{
    QPainter p(&img);
    p.setPen(QPen(color, pen));
    p.drawLine(p1, p2);
    p.drawLine(p2, p3);
    p.drawLine(p3, p1);
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::picStarsMarks(QImage &img,
                         const ArtifactVector &a,
                         const QColor &color,
                         const int pen)
{
    if(a.empty())    return;
    ArtifactVector::const_iterator it = a.constBegin();
    for(; it != a.constEnd(); ++it)
    {
        draw::crossbuck(img,
                        it->center(),
                        (int)it->magnitude(),
                        color, pen);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::catStarsMarks(QImage &img,
                         const ArtifactVector &a,
                         const QColor &color,
                         const int pen)
{
    if(a.empty())    return;
    ArtifactVector::const_iterator it = a.constBegin();
    for(; it < a.constEnd(); ++it)
    {
        draw::cross(img,
                    it->center(),
                    (int)it->magnitude(),
                    color, pen);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::starConfig(QImage &img,
                      const ArtifactVector &a,
                      const QColor &color,
                      const int pen)
{
    if(a.empty())    return;
    QPainter p(&img);
    p.setPen(QPen(color, pen));
    ArtifactVector::const_iterator it = a.constBegin();
    Artifact refStar = *it;
    ++it;
    for(; it < a.constEnd(); ++it)
    {
        p.drawLine(refStar.center(), it->center());
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::targetMarks(QImage &img,
                       const ArtifactVector &a,
                       const QColor &color,
                       const int pen)
{
    if(a.empty())    return;
    QPainter p(&img);
    const int size = 10;
    p.setPen(QPen(color, pen));
    const int startAngle = 0 * 16;
    const int spanAngle = 360 * 16;
    ArtifactVector::const_iterator it = a.constBegin();
    for(; it < a.constEnd(); ++it)
    {
        QRectF rect(QPointF(it->center().x() - size,
                            it->center().y() - size),
                    QPointF(it->center().x() + size,
                            it->center().y() + size));
        p.drawArc(rect, startAngle, spanAngle);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//�������� ��������� ������ �� �������� (��.�.) � ��������
void draw::convertStarMagn(ArtifactVector &a)
{
    if(a.empty())    return;
    double m;
    ArtifactVector::iterator it = a.begin();
    for(; it < a.end(); ++it)
    {
        m = it->magnitude();
        it->setMagnitude(ac::calcStarRadius(m));
    }
}
/////////////////////////////////////////////////////////////////////////////////////
