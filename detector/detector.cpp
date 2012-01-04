#include "detector.h"
/////////////////////////////////////////////////////////////////////////////////////
Detector::Detector(QSettings *s,
                   FrameBox *f,
                   ArtifactBox *stars,
                   ArtifactBox *target) :
    _settings(s),
    _frame(f),
    _stars(stars),
    _target(target),
    _binEnabled(true),
    _mode(TARGET_DETECTION)
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
    detection::findArtifacts(_cache_Frame.data(),
                             _cache_Stars.data(),
                             _magnThresh);
    detection::deleteTarget(_cache_Stars.data(),
                            _cache_Target.data());
    _cache_Stars.setTimeMarker(_cache_Frame.timeMarker());
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::inputFrame(FrameBox *f,
                          int xTarget,
                          int yTarget)
{
    _cache_Target.data().clear();
    _cache_Target.data().push_back(Artifact(xTarget, yTarget));
    _cache_Target.refreshTime();

    f->lock().lockForRead();
    _cache_Frame = *f;
    f->lock().unlock();

    switch(_mode)
    {
    case TARGET_DETECTION:
        this->detectTarget();
        break;
    case STAR_DETECTION:
        this->detectStars();
        break;
    }

    _frame->lock().lockForWrite();
    *_frame = _cache_Frame;
    _frame->lock().unlock();
    emit sendFrame(_frame);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::detectStars()
{
    detection::smooth(_cache_Frame.data(), 7);
    _cache_Frame = _accum.add(_cache_Frame);

    if(_binEnabled)    detection::threshold(_cache_Frame.data());

    this->cookArtifacts();

    _stars->lock().lockForWrite();
    *_stars = _cache_Stars;
    _stars->lock().unlock();

    _target->lock().lockForWrite();
    *_target = _cache_Target;
    _target->lock().unlock();

    emit screenStarsReady(_stars, _target);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::detectTarget()
{
    //work
    _mode = STAR_DETECTION;

    _target->lock().lockForWrite();
    *_target = _cache_Target;
    _target->lock().unlock();
    emit targetDetected(_target);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
