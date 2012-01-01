#include "detector.h"
/////////////////////////////////////////////////////////////////////////////////////
Detector::Detector(QSettings *s,
                   Frame *f,
                   ArtifactBox *a) :
    _settings(s),
    _frame(f),
    _artifactBox(a),
    _binEnabled(true)
{
    this->moveToThread(this);
    this->loadSettings(_settings);
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
Detector::~Detector()
{
    this->quit();
    this->wait();
    this->saveSettings(_settings);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::loadSettings(QSettings *s)
{
    int cap = s->value(__skeyAccumCapacity, Accumulator::_defaultCapacity).toInt();
    _accum.setCapacity(cap);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::saveSettings(QSettings *s)
{
    s->setValue(__skeyAccumCapacity, _accum.capacity());
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::cookArtifacts()
{
    detection::findArtifacts(_rawFrame,
                             _artifactBox->data(),
                             _magnThresh);
    detection::deleteTarget(_artifactBox->data(),
                            _target.center());
    _artifactBox->setTimeMarker(_rawFrame.timeMarker());
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

    detection::smooth(_rawFrame, 7);
    _rawFrame = _accum.add(_rawFrame);
    if(_binEnabled)    detection::threshold(_rawFrame);

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

