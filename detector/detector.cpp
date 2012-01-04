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
void Detector::inputFrame(FrameBox *f)
{
    f->lock().lockForRead();
    _cache_Frame = *f;
    f->lock().unlock();

    switch(_mode)
    {
    case TARGET_DETECTION:
        this->detectTargets();
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
    if(_accum.isFull())    _accum.clear();

    if(_binEnabled)    detection::threshold(_cache_Frame.data());

    detection::findTargets(_cache_Frame.data(),
                           _cache_Stars.data());
    _cache_Stars.setTimeMarker(_cache_Frame.timeMarker());

    _stars->lock().lockForWrite();
    *_stars = _cache_Stars;
    _stars->lock().unlock();
    emit starsReady(_stars);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::detectTargets()
{
    detection::smooth(_cache_Frame.data(), 7);
    _cache_Frame = _accum.add(_cache_Frame);
    if(_binEnabled)    detection::threshold(_cache_Frame.data());
    if(_accum.isFull())
    {
        _accum.clear();
        detection::smooth(_cache_Frame.data(), 7);
        detection::threshold(_cache_Frame.data());
        detection::findTargets(_cache_Frame.data(),
                               _cache_Targets.data());
        if(_cache_Targets.data().empty())
        {
            return;
        }
        _cache_Targets.setTimeMarker(_cache_Frame.timeMarker());
        //_mode = STAR_DETECTION; //mode switch
        _targets->lock().lockForWrite();
        *_targets = _cache_Targets;
        _targets->lock().unlock();
        emit targetsReady(_targets);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
