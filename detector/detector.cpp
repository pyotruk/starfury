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
    _minSquare = s->value(__skeyMinSquare, _defaultMinSquare).toInt();
    _oblongDiff = s->value(__skeyOblongDiff, _defaultOblongDiff).toInt();
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::saveSettings(QSettings *s)
{
    s->setValue(__skeyAccumCapacity, _accum.capacity());
    s->setValue(__skeyDetectorMode, _mode);
    s->setValue(__skeyMinSquare, _minSquare);
    s->setValue(__skeyOblongDiff, _oblongDiff);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::inputFrame(FrameBox *f)
{
    QTime t;
    t.start();

    f->lock().lockForRead();
    _cache_Frame = *f;
    f->lock().unlock();    
    qDebug() << "Detector: RAW frame locking delay = " << t.restart();

    this->accumulate(_mode);
    if(_accum.isFull())    this->accumIsFull();
    qDebug() << "Detector: proc delay = " << t.restart();

    _frame->lock().lockForWrite();
    *_frame = _cache_Frame;
    _frame->lock().unlock();
    qDebug() << "Detector: GUI frame locking delay = " << t.elapsed();

    emit sendFrame(_frame);
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::accumulate(const MODE mode)
{
    QTime t;
    t.start();

    cvwrap::medianBlur(_cache_Frame.data().asCvMat(), _blurKernelSize);
    qDebug() << "Detector: accumulate(): medianBlur() delay = " << t.restart();

    switch(mode)
    {
    case TARGET_DETECTION:
        _cache_Frame = _accum.add(_cache_Frame);
        break;
    case STAR_DETECTION:
        if(_velocity.isNull())    return;
        _cache_Frame = _accum.add(_cache_Frame, _velocity);
        break;
    }
    qDebug() << "Detector: accumulate(): _accum.add() delay = " << t.restart();

    if(_binEnabled)    cvwrap::otsuThreshold(_cache_Frame.data().asCvMat());
    qDebug() << "Detector: accumulate(): otsuThreshold() delay = " << t.elapsed();
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::detectStars()
{
    cvwrap::otsuThreshold(_cache_Frame.data().asCvMat());
    detector_hf::findTargets(_cache_Frame.data(),
                             _cache_Stars.data(),
                             _minSquare,
                             true,
                             _oblongDiff);
    if(_cache_Stars.data().empty())
    {
        qDebug() << "Detector says: [_cache_Stars] is empty!";
        return;
    }
    _cache_Stars.setTimeMarker(_cache_Frame.timeMarker());
    _stars->lock().lockForWrite();
    *_stars = _cache_Stars;
    _stars->lock().unlock();
    emit starsReady(_stars);
    qDebug() << "Detector: emit starsReady(), N =" << _cache_Stars.data().size();
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::detectTargets()
{
    cvwrap::medianBlur(_cache_Frame.data().asCvMat(), _blurKernelSize);
    cvwrap::otsuThreshold(_cache_Frame.data().asCvMat());
    detector_hf::findTargets(_cache_Frame.data(),
                             _cache_Targets.data(),
                             _minSquare,
                             true,
                             _oblongDiff);
    if(_cache_Targets.data().empty())
    {
        qDebug() << "Detector: [_cache_Targets] is empty!";
        return;
    }
    _cache_Targets.setTimeMarker(_cache_Frame.timeMarker());
    _targets->lock().lockForWrite();
    *_targets = _cache_Targets;
    _targets->lock().unlock();
    emit targetsReady(_targets);
    qDebug() << "Detector: emit targetsReady(), N =" << _cache_Targets.data().size();
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::inputScreenVelocity(const double vx,
                                   const double vy)
{
    _velocity = QPointF(vx, vy);
    qDebug() << "Detector: velocity = " << _velocity;
}
/////////////////////////////////////////////////////////////////////////////////////
void Detector::accumIsFull()
{
    QTime t;
    t.start();

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

    qDebug() << "Detector: accumIsFull(): detection delay = " << t.elapsed();
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
