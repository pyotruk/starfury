#include "detector_hf.h"
/////////////////////////////////////////////////////////////////////////////////////
bool isOblong(const cv::Rect &rect,
              const int oblongDiff)
{
    if(qAbs(rect.width - rect.height) > oblongDiff)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void detector_hf::findTargets(Frame &f,                 //input
                              ArtifactVector &av,       //output
                              const int minSquare,
                              const bool MTI,           /* Moving-Target Indication (селекция движущихся целей)
                                                           только для накопленного кадра */
                              const int oblongDiff)     /* распознавание вытянутых треков
                                                           от движущихся целей */
{
    av.clear();
    const quint32 floodColor = 0xFF - 1;
    cv::Rect rect;
    QPoint center;
    const uchar *pix;
    for(int y = 0; y < f.header().height(); ++y)
    {
        for(int x = 0; x < f.header().width(); ++x)
        {
            pix = f.constPnt(x, y);
            if(*pix > floodColor)
            {
                cv::floodFill(f.asCvMat(), cv::Point(x, y), floodColor, &rect);
                int sq = cvwrap::square(rect);
                if(sq < minSquare)    continue;

                if(MTI)
                {
                    if(isOblong(rect, oblongDiff))    continue;
                }

                cvwrap::calcRectCenter(rect, center);
                av.push_back(Artifact(center, (double)rect.width));
            }
        }
    }
    if(av.empty())    return;
    qSort(av.begin(), av.end(), qGreater<Artifact>());
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
