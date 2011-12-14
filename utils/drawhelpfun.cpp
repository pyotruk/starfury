#include "drawhelpfun.h"
/////////////////////////////////////////////////////////////////////////////////////
void draw::crossbuck(QImage &img,
                     const QPoint &center,
                     const int side,
                     const QColor &color)
{
    const int side2 = side / 2;
    QPainter painter(&img);
    painter.setPen(QPen(color, 1));
    painter.drawLine(QPoint(center.x() - side2, center.y() - side2),
                     QPoint(center.x() + side2, center.y() + side2));
    painter.drawLine(QPoint(center.x() + side2, center.y() - side2),
                     QPoint(center.x() - side2, center.y() + side2));
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::cross(QImage &img,
                 const QPoint &center,
                 const int side,
                 const QColor &color)
{
    const int side2 = side / 2;
    QPainter painter(&img);
    painter.setPen(QPen(color, 1));
    painter.drawLine(QPoint(center.x(), center.y() - side2),
                     QPoint(center.x(), center.y() + side2));
    painter.drawLine(QPoint(center.x() - side2, center.y()),
                     QPoint(center.x() + side2, center.y()));
}
/////////////////////////////////////////////////////////////////////////////////////
void draw::triangle(QImage &img,
                    const int width,
                    const QColor &color,
                    const QPoint &p1,
                    const QPoint &p2,
                    const QPoint &p3)
{
    QPainter p(&img);
    p.setPen(QPen(color, width));
    p.drawLine(p1, p2);
    p.drawLine(p2, p3);
    p.drawLine(p3, p1);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
