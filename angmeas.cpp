#include "angmeas.h"
/////////////////////////////////////////////////////////////////////////////////////
AngMeas::AngMeas(QSettings *settings) :
    _settings(settings),
    _frame(new Frame),
    _artVec(new ArtifactVector),
    _mutex(new QMutex)
{
    this->moveToThread(this);
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
AngMeas::~AngMeas()
{
    this->exit();
    this->terminate();
    delete _mutex;
    delete _artVec;
    delete _frame;
}
/////////////////////////////////////////////////////////////////////////////////////
void AngMeas::frameIn(Frame  *frame,
                      QMutex *mutex)
{
    if(mutex->tryLock(_timeout))
    {
        *_frame = *frame;
        mutex->unlock();

        if(this->_mutex->tryLock(_timeout))
        {
            processing(_frame);
            this->_mutex->unlock();
            emit artifactsOut(_artVec, _mutex);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void AngMeas::processing(Frame *frame)
{
    cv::blur(frame->asCvMat(),
             frame->asCvMat(),
             cv::Size(5, 5));
    cv::threshold(frame->asCvMat(),
                  frame->asCvMat(),
                  0,
                  0xFF,
                  cv::THRESH_OTSU);
    this->findArtifacts(frame);
}
/////////////////////////////////////////////////////////////////////////////////////
void AngMeas::findArtifacts(Frame *frame)
{
    _artVec->clear();
    quint32 floodColor = 0x7F;
    uchar *p0 = frame->data();
    uchar *pix;
    cv::Rect rect;
    QPoint center;
    cv::Mat cvmat = frame->asCvMat();
    Artifact art;
    int width  = frame->header().width;
    int height = frame->header().height;
    int depth  = frame->header().depth;
    for(int y=0; y < height; ++y)
    {
        for(int x=0; x < width; ++x)
        {
            pix = p0 + depth * (width * y + x);
            if(*pix > floodColor)
            {
                cv::floodFill(cvmat, cv::Point(x, y), floodColor, &rect);
                if(rect.width > _magnThresh)
                {
                    calcRectCenter(rect, center);
                    art.setCenter(center);
                    art.setMagnitude((double&)rect.width);
                    _artVec->push_back(art);
                }
            }
        }
    }
    qSort(*_artVec);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
