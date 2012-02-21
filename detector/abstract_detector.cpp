#include "abstract_detector.h"
/////////////////////////////////////////////////////////////////////////////////////
AbstractDetector::AbstractDetector(QSettings *s) :
    _settings(s),
    _ready(false)
{
    this->moveToThread(this);
    this->loadSettings(_settings);
}
/////////////////////////////////////////////////////////////////////////////////////
AbstractDetector::~AbstractDetector()
{
    this->wait();
    this->saveSettings(_settings);
}
/////////////////////////////////////////////////////////////////////////////////////
void AbstractDetector::loadSettings(QSettings *s)
{
    int cap = s->value(__skeyAccumCapacity, Accumulator::_defaultCapacity).toInt();
    _accum.setCapacity(cap);
    _minSquare = s->value(__skeyMinSquare, _defaultMinSquare).toInt();
    _maxOblong = s->value(__skeyOblongDiff, _defaultOblongDiff).toInt();
}
/////////////////////////////////////////////////////////////////////////////////////
void AbstractDetector::saveSettings(QSettings *s)
{
    s->setValue(__skeyAccumCapacity, _accum.capacity());
    s->setValue(__skeyMinSquare, _minSquare);
    s->setValue(__skeyOblongDiff, _maxOblong);
}
/////////////////////////////////////////////////////////////////////////////////////
void AbstractDetector::inputFrame(FrameBox *f)
{
    _cache_Frame = *f;
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
void AbstractDetector::run()
{
    _mutex.lock();
    cvwrap::medianBlur(_cache_Frame.data().asCvMat(), _blurKernelSize);
    this->accumulate();
    if(_accum.isFull())
    {
        _cache_Frame = _accum.frame();
        _accum.clear();
        this->detect();
        if(_artifacts.data().empty())
        {
            qDebug() << "AbstractDetector: [_artifacts] is empty!";
        }
        else
        {
            _ready = true;
        }
    }
    _mutex.unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
const FrameBox& AbstractDetector::frame()
{
    return _accum.frame();
}
/////////////////////////////////////////////////////////////////////////////////////
const ArtifactBox& AbstractDetector::artifacts()
{
    _ready = false;
    return _artifacts;
}
/////////////////////////////////////////////////////////////////////////////////////
void AbstractDetector::detect()
{
    cvwrap::otsuThreshold(_cache_Frame.data().asCvMat());
    detector_hf::findTargets(_cache_Frame.data(),
                             _artifacts.data(),
                             _minSquare,
                             true,
                             _maxOblong);
    _artifacts.setTimeMarker(_cache_Frame.timeMarker());
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
