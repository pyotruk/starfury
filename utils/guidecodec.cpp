#include "guidecodec.h"
/////////////////////////////////////////////////////////////////////////////////////
static const double __pi = 3.1415926535897932384626433832795;
/////////////////////////////////////////////////////////////////////////////////////
void  decodeADM( double &a, //rad
                 double &d, //rad
                 double &m,
                 const uchar  *x)
{
    const double del = 2*__pi/16/1024/1024;

    Tib4 y;
    y.i = 0;

    y.b[0] = x[0];
    y.b[1] = x[1];
    y.b[2] = x[2];
    a = del * y.i;

    y.b[0] = x[3];
    y.b[1] = x[4];
    y.b[2] = x[5];
    d = del * y.i - __pi/2;

    m = x[6] / 10.0;
}
/////////////////////////////////////////////////////////////////////////////////////
void guidecat::decodeStar(Star &star,
                          const uchar *data)
{
    double alpha, delta, magn;
    decodeADM(alpha, delta, magn, data);
    star = Star(alpha, delta, magn);
}
/////////////////////////////////////////////////////////////////////////////////////
qint64 calcWorkBlockSize(const double segmentHeight)
{
    int nBlocks = qCeil(segmentHeight / __deltaPerBlock); //���-�� ������ ��������
    return qFloor(0.5 + nBlocks * __starsPerBlock * __bytesPerStar * __marginKoef);
}
/////////////////////////////////////////////////////////////////////////////////////
void guidecat::calcPositionRange(const qint64 streamSize,
                                 const double lowerDelta,
                                 const double upperDelta,
                                 qint64 &startPos,
                                 qint64 &finPos)
{
    /*������������ ������������� ������� �������� [ delta(position) ],
     ��������� ������������ :)      */
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
    needPosition *= __bytesPerStar;
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
