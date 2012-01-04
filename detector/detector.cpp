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
void Detector::cookArtifacts()
{
    detection::findArtifacts(_cache_Frame.data(),
                             _cache_Stars.data(),
                             _magnThresh);
    detection::deleteTarget(_cache_Stars.data(),
                            _cache_Targets.data());
    _cache_Stars.setTimeMarker(_cache_Frame.timeMarker());
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
void Detector::inputStrobPos(const int xTarget,
                             const int yTarget)
{
    if(_mode == STAR_DETECTION)
    {
        _cache_Targets.data().clear();
        _cache_Targets.data().push_back(Artifact(xTarget, yTarget));
        _cache_Targets.refreshTime();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::detectStars()
{
    detection::smooth(_cache_Frame.data(), 7);
    _cache_Frame = _accum.add(_cache_Frame);

    if(_binEnabled)    detection::threshold(_cache_Frame.data());

    this->cookArtifacts();

    _targets->lock().lockForWrite();
    _stars->lock().lockForWrite();
    *_stars = _cache_Stars;
    *_targets = _cache_Targets;
    _stars->lock().unlock();
    _targets->lock().unlock();
    emit artifactsReady(_stars, _targets);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::detectTargets()
{
    //work
    _mode = STAR_DETECTION;

    _targets->lock().lockForWrite();
    *_targets = _cache_Targets;
    _targets->lock().unlock();
    emit targetsDetected(_targets);

    if(!_cache_Targets.data().empty())
    {
        emit setStrobPos((int)_cache_Targets.data().front().center().x(),
                         (int)_cache_Targets.data().front().center().y());
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
