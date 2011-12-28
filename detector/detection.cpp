#include "detection.h"
/////////////////////////////////////////////////////////////////////////////////////
void detection::filtering(Frame &f)
{
    cv::blur(f.asCvMat(),
             f.asCvMat(),
             cv::Size(5, 5));
    cv::threshold(f.asCvMat(),
                  f.asCvMat(),
                  0,
                  0xFF,
                  cv::THRESH_OTSU);
}
/////////////////////////////////////////////////////////////////////////////////////
void detection::findArtifacts(Frame &f,
                              ArtifactVector &a,
                              const double magnThresh)
{
    a.clear();
    quint32 floodColor = 0xFF - 1;
    double magn;
    cv::Rect rect;
    QPoint center;
    cv::Mat cvmat = f.asCvMat();
    Artifact art;
    int width  = f.header().width;
    int height = f.header().height;
    int depth  = f.header().depth;
    uchar *p0 = f.data();
    uchar *pix;
    for(int y=0; y < height; ++y)
    {
        for(int x=0; x < width; ++x)
        {
            pix = p0 + depth * (width * y + x);
            if(*pix > floodColor)
            {
                cv::floodFill(cvmat, cv::Point(x, y), floodColor, &rect);
                magn = (double)rect.width;
                if(magn > magnThresh)
                {
                    cvhelp::calcRectCenter(rect, center);
                    art.setCenter(center);
                    art.setMagnitude(magn);
                    a.push_back(art);
                }
            }
        }
    }
    qSort(a.begin(), a.end(), qGreater<Artifact>());
}
/////////////////////////////////////////////////////////////////////////////////////
void detection::deleteTarget(ArtifactVector &a,
                             const QPointF &target)
{
    const double eps = 2;
    double dist;
    double minDist = 1000000;
    int targetIndex;
    ArtifactVector::iterator it = a.begin();
    for(int i=0; it < a.end(); ++it, ++i)
    {
        dist = ac::calcDistance(target, it->center());
        if(dist < minDist)
        {
            minDist = dist;
            targetIndex = i;
            if(minDist < eps)   break;
        }
    }
    a.remove(targetIndex);
}
/////////////////////////////////////////////////////////////////////////////////////
bool detection::isDoubleStar(const cv::Rect &rect)
{
    const int maxHeightWidthDiff = 1;
    if(qAbs(rect.width - rect.height) > maxHeightWidthDiff)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
