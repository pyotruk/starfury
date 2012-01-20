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
               const QColor&,
               const int pen);
void cross(QImage&,
           const QPointF &center,
           const int side,
           const QColor&,
           const int pen);
void triangle(QImage&,
              const QPointF &p1,
              const QPointF &p2,
              const QPointF &p3,
              const QColor&,
              const int pen);
/////////////////////////////////////////////////////////////////////////////////////
void picStarsMarks(QImage&,
                   const ArtifactVector&,
                   const QColor&,
                   const int pen);
void catStarsMarks(QImage&,
                   const ArtifactVector&,
                   const QColor&,
                   const int pen);
void starConfig(QImage&,
                const ArtifactVector&,
                const QColor&,
                const int pen);
void targetMarks(QImage&,
                 const ArtifactVector&,
                 const QColor&,
                 const int pen);
/////////////////////////////////////////////////////////////////////////////////////
void convertStarMagn(ArtifactVector&); //пересчёт магнитуды звезды из каталога (зв.в.) в картинку
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // DRAWING_H
