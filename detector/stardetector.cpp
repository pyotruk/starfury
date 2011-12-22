#include "stardetector.h"
/////////////////////////////////////////////////////////////////////////////////////
StarDetector::StarDetector(QSettings   *s,
                           ArtifactBox *a) :
    _settings(s),
    _artifactBox(a)
{
    this->moveToThread(this);
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
StarDetector::~StarDetector()
{
    this->quit();
    this->wait();
}
/////////////////////////////////////////////////////////////////////////////////////
void StarDetector::inputFrame(Frame *f,
                              int xTarget,
                              int yTarget)
{
    _target.setCenter(QPointF(xTarget, yTarget));

    f->lock().lockForRead();
    _frame = *f;
    f->lock().unlock();

    this->filtering(_frame);

    _artifactBox->lock().lockForWrite();
    this->findArtifacts(_frame,
                        _artifactBox->data(),
                        _magnThresh);
    this->deleteTarget(_artifactBox->data(),
                       _target.center());

    QDateTime tMark = _artifactBox->timeMarker();
    timeutils::winfiletime2qdatetime(_frame.header().timeID, tMark);
    _artifactBox->setTimeMarker(tMark);
    _artifactBox->lock().unlock();

    emit screenStarsReady(_artifactBox,
                          _target.center().x(),
                          _target.center().y());
}
/////////////////////////////////////////////////////////////////////////////////////
void StarDetector::filtering(Frame &f)
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
void StarDetector::findArtifacts(Frame &f,
                                 ArtifactVector &a,
                                 const double magnThresh)
{
    a.clear();
    quint32 floodColor = 0x7F;
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
//                    if(!this->isDoubleStar(rect, _maxHeightWidthDiff))
//                    {
                        art.setCenter(center);
                        art.setMagnitude(magn);
                        a.push_back(art);
//                    }
//                    else
//                    {
//                        qDebug() << "double star detected:  x = " << center.x() << "    y = " << center.y();
//                    }
                }
            }
        }
    }
    qSort(a.begin(), a.end(), qGreater<Artifact>());
}
/////////////////////////////////////////////////////////////////////////////////////
void StarDetector::deleteTarget(ArtifactVector &a,
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
bool StarDetector::isDoubleStar(const cv::Rect &rect,
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
/////////////////////////////////////////////////////////////////////////////////////
