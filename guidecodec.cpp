#include "guidecodec.h"
/////////////////////////////////////////////////////////////////////////////////////
void  decodeADM( double &a, //rad
                 double &d, //rad
                 double &m,
                 char   *x)
{
    const double del = 2*pi/16/1024/1024;
    Tib4 y;
    y.i = 0;

    y.b[0] = x[0];
    y.b[1] = x[1];
    y.b[2] = x[2];
    a = del * y.i;

    y.b[0] = x[3];
    y.b[1] = x[4];
    y.b[2] = x[5];
    d = del * y.i - pi/2;

    m = x[6] / 10;
}
/////////////////////////////////////////////////////////////////////////////////////
void decodeStar(Star &star, char *data)
{
    double alpha, delta, magn;
    decodeADM(alpha, delta, magn, data);
    star = Star(alpha, delta, magn);
}
/////////////////////////////////////////////////////////////////////////////////////
qint64 calcWorkBlockSize(const double segmentHeight)
{
    int nBlocks = qCeil(segmentHeight / BLOCK_SIZE_DELTA); //кол-во блоков каталога
    return qFloor(0.5 + nBlocks * BLOCK_SIZE_STARS * BYTES_PER_STAR * MARGIN_KOEF);
}
/////////////////////////////////////////////////////////////////////////////////////
void calcPositionRange(const qint64 streamSize,
                       const double lowerDelta,
                       const double upperDelta,
                       qint64 &startPos,
                       qint64 &finPos)
{
    /*Коэффициенты аппроксимации функции каталога [ delta(position) ],
     подобраны эвристически :)      */
    const double threshold = 0.9;
    const double A1 = -1.26e-7;
    const double B1 = 1.55;
    const double C1 = -0.4;
    const double D1 = 2.0;
    const double A2 = -1.27e-7;
    const double B2 = 1.15;
    double delta = (lowerDelta + upperDelta ) / 2;
    qint64 needPosition;
    if(delta > threshold)   needPosition = qFloor(0.5 + (delta - B1 - C1 * qSin(D1 * delta)) / A1);
    else                    needPosition = qFloor(0.5 + (delta - B2) / A2);
    needPosition *= BYTES_PER_STAR;
    qint64 workBlockSize = calcWorkBlockSize(upperDelta - lowerDelta);
    startPos = needPosition - workBlockSize / 2;
    if(startPos < 0)     startPos = 0;
    finPos = needPosition + workBlockSize / 2;
    if(finPos > streamSize)     finPos = streamSize;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
