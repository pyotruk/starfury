#include "drawhelpfun.h"
/////////////////////////////////////////////////////////////////////////////////////
void drawCrossbuck(QImage &img,
                   const QPoint &center,
                   const int side,
                   const QColor &color)
{
    const int side2 = side / 2;
    QPainter painter(&img);
    painter.setPen(QPen(color, 2));
    painter.drawLine(QPoint(center.x() - side2, center.y() - side2),
                     QPoint(center.x() + side2, center.y() + side2));
    painter.drawLine(QPoint(center.x() + side2, center.y() - side2),
                     QPoint(center.x() - side2, center.y() + side2));
}
/////////////////////////////////////////////////////////////////////////////////////
void drawCross(QImage &img,
               const QPoint &center,
               const int side,
               const QColor &color)
{
    const int side2 = side / 2;
    QPainter painter(&img);
    painter.setPen(QPen(color, 2));
    painter.drawLine(QPoint(center.x(), center.y() - side2),
                     QPoint(center.x(), center.y() + side2));
    painter.drawLine(QPoint(center.x() - side2, center.y()),
                     QPoint(center.x() + side2, center.y()));
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
