#include "detector_hf.h"
/////////////////////////////////////////////////////////////////////////////////////
void detector_hf::smooth(Frame &f,
                         const int kernelSize)
{
    cv::medianBlur(f.asCvMat(),
                   f.asCvMat(),
                   kernelSize);
}
/////////////////////////////////////////////////////////////////////////////////////
void detector_hf::findArtifacts(Frame &f,
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
    int width  = f.header().width();
    int height = f.header().height();
    const uchar *pix;
    for(int y=0; y < height; ++y)
    {
        for(int x=0; x < width; ++x)
        {
            pix = f.constPnt(x, y);
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
bool detector_hf::isOblong(const cv::Rect &rect,
                           const int maxHeightWidthDiff)
{
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
void detector_hf::findTargets(Frame &f,
                              ArtifactVector &a)
{
    const int maxHeightWidthDiff = 2;
    const int minSquare = 9;
    a.clear();
    quint32 floodColor = 0xFF - 1;
    cv::Rect rect;
    QPoint center;
    cv::Mat cvmat = f.asCvMat();
    Artifact art;
    int width  = f.header().width();
    int height = f.header().height();
    const uchar *pix;
    for(int y=0; y < height; ++y)
    {
        for(int x=0; x < width; ++x)
        {
            pix = f.constPnt(x, y);
            if(*pix > floodColor)
            {
                cv::floodFill(cvmat, cv::Point(x, y), floodColor, &rect);
                if(cvhelp::square(rect) < minSquare)    continue;
                if(isOblong(rect, maxHeightWidthDiff))    continue;
                cvhelp::calcRectCenter(rect, center);
                art.setCenter(center);
                art.setMagnitude((double)rect.width);
                a.push_back(art);
            }
        }
    }
    if(a.empty())    return;
    qSort(a.begin(), a.end(), qGreater<Artifact>());
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
