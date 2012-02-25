/* Интерфейсный модуль астрометрии.
   Реализует отождествление звёздных конфигураций методами:
   свободных векторов [FREEVEC], подобных треугольников [SIMTRI]. */
#ifndef ASTROMETRY_H
#define ASTROMETRY_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QSize>
#include <QSizeF>
#include <QPointF>
/////////////////////////////////////////////////////////////////////////////////////
#include "com/telescope.h"
#include "boxes/artifact.h"
#include "boxes/star.h"
#include "math/astrocalc.h"
#include "astrometry/simtri/triangle.h"
#include "astrometry/freevec/freevec.h"
/////////////////////////////////////////////////////////////////////////////////////
namespace astrometry
{
//return values
enum RETURN_VALUES {SUCCESS = 0,
                    TOO_LESS_RAW_STARS     = -1,  //less than __minStarQuantity
                    TOO_LESS_EQUATED_STARS = -2,  //less than minEquatedStarQuantity
                    UNKNOWN_ERROR          = -3}; //when switch default (it is impossible)
//methods
enum METHOD {SIMTRI = 0, FREEVEC = 1};
//
static const int _minStarQuantity = 3;
//
/* Отождествляет звёзды из двух массивов [picStars] [catStars].
   После обработки входные массивы содержат только отождествлённые
   звёзды в соответсвующем порядке.
   Не забывай проверять return values. */
int equate(ArtifactVector &picStars,    //звёзды с картинки
           ArtifactVector &catStars,    //звёзды из каталога
           const QSize    &screen,      //размер кадра
           const double    similarEps,  //точность подобия конфигураций (точность равенства _отношений_ сторон)
           const double    nearStarDist,    /* макс. расстояние (в экранной СК) между двумя звёздами,
                                               при котором звёзды считаются близкими и выкидываются из обработки */
           const int       maxStarQuantity,         //верхняя граница для кол-ва обрабатываемых звёзд
           const int       minEquatedStarQuantity,  //нижняя граница для кол-ва отождествлённых звёзд
           METHOD          method);                 //метод отождествления

/* Следующие функции выполняют преобразование координат каталожной звезды [alpha, delta]
   в координаты экранной звезды [x, y] и обратно.
   В текущем варианте _не_ учитывается поворот поля. */
void catStar2screenStar(const Star   &s,    //каталожная звезда
                        Artifact     &a,    //экранная звезда
                        const TelPos &t,    //вектор состояния телескопа
                        const QSizeF &field,    //размер поля [рад]
                        const QSize  &screen);  //размер кадра
void screenStar2catStar(const Artifact &a,
                        Star           &s,
                        const TelPos   &t,
                        const QSizeF   &field,
                        const QSize    &screen);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // ASTROMETRY_H
