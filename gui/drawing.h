#ifndef DRAWING_H
#define DRAWING_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QImage>
#include <QPainter>
#include <QPointF>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/artifact.h"
/////////////////////////////////////////////////////////////////////////////////////
namespace draw
{
void crossbuck(QImage&,
               const QPointF &center,
               const int side,
               const QColor&);
void cross(QImage&,
           const QPointF &center,
           const int side,
           const QColor &color);
void triangle(QImage&,
              const int width,
              const QColor&,
              const QPointF &p1,
              const QPointF &p2,
              const QPointF &p3);
/////////////////////////////////////////////////////////////////////////////////////
void artifactMarks(QImage&,
                   ArtifactVector&);
void starMarks(QImage&,
               ArtifactVector&);
void starConfig(QImage&,
                ArtifactVector&,
                const int width,
                const QColor&);
/////////////////////////////////////////////////////////////////////////////////////
void convertStarMagn(ArtifactVector&); //�������� ��������� ������ �� �������� (��.�.) � ��������
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // DRAWING_H