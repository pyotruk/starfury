#include "detector.h"
/////////////////////////////////////////////////////////////////////////////////////
Detector::Detector(QSettings *s,
                   FrameBox *f,
                   ArtifactBox *stars,
                   ArtifactBox *targets) :
    _settings(s),
    _frame(f),
    _stars(stars),
    _targets(targets),
    _binEnabled(true),
    _velocity(0, 0)
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
    _mode = (MODE)s->value(__skeyDetectorMode, _defaultMode).toInt();
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::saveSettings(QSettings *s)
{
    s->setValue(__skeyAccumCapacity, _accum.capacity());
    s->setValue(__skeyDetectorMode, _mode);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::inputFrame(FrameBox *f)
{
    f->lock().lockForRead();
    _cache_Frame = *f;
    f->lock().unlock();

    this->accumulate(_mode);
    if(_accum.isFull())    this->accumIsFull();

    _frame->lock().lockForWrite();
    *_frame = _cache_Frame;
    _frame->lock().unlock();
    emit sendFrame(_frame);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::accumulate(const MODE mode)
{
    detection::smooth(_cache_Frame.data(), _smoothingKernelSize);

    switch(mode)
    {
    case TARGET_DETECTION:
        _cache_Frame = _accum.add(_cache_Frame);
        break;
    case STAR_DETECTION:
        _cache_Frame = _accum.add(_cache_Frame, _velocity);
        break;
    }

    if(_binEnabled)    detection::threshold(_cache_Frame.data());
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::detectStars()
{
    detection::threshold(_cache_Frame.data());
    detection::findTargets(_cache_Frame.data(),
                           _cache_Stars.data());
    if(_cache_Stars.data().empty())
    {
        return;
    }
    _cache_Stars.setTimeMarker(_cache_Frame.timeMarker());
    _stars->lock().lockForWrite();
    *_stars = _cache_Stars;
    _stars->lock().unlock();
    emit starsReady(_stars);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::detectTargets()
{
    detection::smooth(_cache_Frame.data(), _smoothingKernelSize);
    detection::threshold(_cache_Frame.data());
    detection::findTargets(_cache_Frame.data(),
                           _cache_Targets.data());
    if(_cache_Targets.data().empty())
    {
        return;
    }
    _cache_Targets.setTimeMarker(_cache_Frame.timeMarker());
    _targets->lock().lockForWrite();
    *_targets = _cache_Targets;
    _targets->lock().unlock();
    emit targetsReady(_targets);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::inputScreenVelocity(const double vx,
                                   const double vy)
{
    _velocity = QPointF(vx, vy);
    qDebug() << "screen velocity: " << vx << "  " << vy;
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::accumIsFull()
{
    _cache_Frame = _accum.frame();
    _accum.clear();

    switch(_mode)
    {
    case TARGET_DETECTION:
        this->detectTargets();
        break;
    case STAR_DETECTION:
        this->detectStars();
        break;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
