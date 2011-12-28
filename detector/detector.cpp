#include "detector.h"
/////////////////////////////////////////////////////////////////////////////////////
Detector::Detector(QSettings *s,
                   Frame *f,
                   ArtifactBox *a) :
    _settings(s),
    _frame(f),
    _artifactBox(a)
{
    this->moveToThread(this);
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
Detector::~Detector()
{
    this->quit();
    this->wait();
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::cookArtifacts()
{
    detection::findArtifacts(_rawFrame,
                             _artifactBox->data(),
                             _magnThresh);
    detection::deleteTarget(_artifactBox->data(),
                            _target.center());

    QDateTime tMark = _artifactBox->timeMarker();
    timeutils::winfiletime2qdatetime(_rawFrame.header().timeID, tMark);
    _artifactBox->setTimeMarker(tMark);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::inputFrame(Frame *f,
                          int xTarget,
                          int yTarget)
{
    _target.setCenter(QPointF(xTarget, yTarget));

    f->lock().lockForRead();
    _rawFrame = *f;
    f->lock().unlock();

    detection::filtering(_rawFrame);

    _artifactBox->lock().lockForWrite();
    this->cookArtifacts();
    _artifactBox->lock().unlock();
    emit screenStarsReady(_artifactBox,
                          _target.center().x(),
                          _target.center().y());

    _frame->lock().lockForWrite();
    *_frame = _rawFrame;
    _frame->lock().unlock();
    emit sendFrame(_frame);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

