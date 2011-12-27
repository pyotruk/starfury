#include "drawing.h"
/////////////////////////////////////////////////////////////////////////////////////
void draw::crossbuck(QImage &img,
                     const QPointF &center,
                     const int side,
                     const QColor &color)
{
    const int side2 = side / 2;
    QPainter painter(&img);
    painter.setPen(QPen(color, 1));
    painter.drawLine(QPointF(center.x() - side2, center.y() - side2),
                     QPointF(center.x() + side2, center.y() + side2));
    painter.drawLine(QPointF(center.x() + side2, center.y() - side2),
                     QPointF(center.x() - side2, center.y() + side2));
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::cross(QImage &img,
                 const QPointF &center,
                 const int side,
                 const QColor &color)
{
    const double side2 = side / 2;
    QPainter painter(&img);
    painter.setPen(QPen(color, 1));
    painter.drawLine(QPointF(center.x(), center.y() - side2),
                     QPointF(center.x(), center.y() + side2));
    painter.drawLine(QPointF(center.x() - side2, center.y()),
                     QPointF(center.x() + side2, center.y()));
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::triangle(QImage &img,
                    const int width,
                    const QColor &color,
                    const QPointF &p1,
                    const QPointF &p2,
                    const QPointF &p3)
{
    QPainter p(&img);
    p.setPen(QPen(color, width));
    p.drawLine(p1, p2);
    p.drawLine(p2, p3);
    p.drawLine(p3, p1);
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::artifactMarks(QImage &img,
                         ArtifactVector &a)
{
    ArtifactVector::iterator it = a.begin();
    for(; it != a.end(); ++it)
    {
        draw::crossbuck(img,
                        it->center(),
                        (int)it->magnitude(),
                        Qt::green);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::starMarks(QImage &img,
                     ArtifactVector &s)
{
    ArtifactVector::iterator it = s.begin();
    for(; it != s.end(); ++it)
    {
        draw::cross(img,
                    it->center(),
                    (int)it->magnitude(),
                    Qt::magenta);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::starConfig(QImage &img,
                      ArtifactVector &a,
                      const int width,
                      const QColor &color)
{
    QPainter p(&img);
    p.setPen(QPen(color, width));
    ArtifactVector::const_iterator it = a.constBegin();
    Artifact refStar = *it;
    ++it;
    for(; it < a.constEnd(); ++it)
    {
        p.drawLine(refStar.center(), it->center());
    }
    a.clear();
}
/////////////////////////////////////////////////////////////////////////////////////
//пересчёт магнитуды звезды из каталога (зв.в.) в картинку
void draw::convertStarMagn(ArtifactVector &a)
{
    double m;
    ArtifactVector::iterator it = a.begin();
    for(; it < a.end(); ++it)
    {
        m = it->magnitude();
        it->setMagnitude(ac::calcStarRadius(m));
    }
}
/////////////////////////////////////////////////////////////////////////////////////
