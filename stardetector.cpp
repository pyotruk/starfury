#include "stardetector.h"
/////////////////////////////////////////////////////////////////////////////////////
StarDetector::StarDetector(QSettings *settings) :
    _settings(settings),
    _frame(new Frame),
    _artVec(new ArtifactVector),
    _mutex(new QMutex)
{
    this->moveToThread(this);
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
StarDetector::~StarDetector()
{
    this->exit();
    this->terminate();
    delete _mutex;
    delete _artVec;
    delete _frame;
}
/////////////////////////////////////////////////////////////////////////////////////
void StarDetector::frameIn(Frame *frame,
                      QMutex *mutex,
                      int xTarget,
                      int yTarget)
{
    _target = QPoint(xTarget, yTarget);
    if(mutex->tryLock(_timeout))
    {
        *_frame = *frame;
        mutex->unlock();

        this->filtering(_frame);
        if(this->_mutex->tryLock(_timeout))
        {
            this->findArtifacts(_frame, _artVec, _magnThresh);
            this->deleteTarget(*_artVec, _target);
            winfiletime2qdatetime(_frame->header().timeID, _time);
            this->_mutex->unlock();
            emit artifactsOut(_artVec, _mutex, &_time);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void StarDetector::filtering(Frame *frame)
{
    cv::blur(frame->asCvMat(),
             frame->asCvMat(),
             cv::Size(5, 5));
    cv::threshold(frame->asCvMat(),
                  frame->asCvMat(),
                  0,
                  0xFF,
                  cv::THRESH_OTSU);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarDetector::findArtifacts(Frame *frame,
                            ArtifactVector *artVec,
                            double thresh)
{
    artVec->clear();
    quint32 floodColor = 0x7F;
    double magn;
    cv::Rect rect;
    QPoint center;
    cv::Mat cvmat = frame->asCvMat();
    Artifact art;
    int width  = frame->header().width;
    int height = frame->header().height;
    int depth  = frame->header().depth;
    uchar *p0 = frame->data();
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
                if(magn > thresh)
                {
                    calcRectCenter(rect, center);
                    art.setCenter(center);
                    art.setMagnitude(magn);
                    artVec->push_back(art);
                }
            }
        }
    }
    qSort(*artVec);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarDetector::deleteTarget(ArtifactVector &artVec,
                           QPoint &target)
{
    const double eps = 2;
    double dist;
    double minDist = 1000000;
    int targetIndex;
    ArtifactVector::iterator it = artVec.begin();
    for(int i=0; it != artVec.end(); ++it, ++i)
    {
        dist = ac::calcDistance(target, it->center());
        if(dist < minDist)
        {
            minDist = dist;
            targetIndex = i;
            if(minDist < eps)   break;
        }
    }
    artVec.remove(targetIndex);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
