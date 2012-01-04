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
                   const ArtifactVector&);
void starMarks(QImage&,
               const ArtifactVector&);
void starConfig(QImage&,
                const ArtifactVector&,
                const int width,
                const QColor&);
void targetMarks(QImage&,
                 const ArtifactVector&,
                 const QColor&);
/////////////////////////////////////////////////////////////////////////////////////
void convertStarMagn(ArtifactVector&); //пересчёт магнитуды звезды из каталога (зв.в.) в картинку
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // DRAWING_H
