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
    _frame = *f;
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
void AbstractDetector::run()
{
    _mutex.lock();
    this->accumulate();
    _frame = _accum.frame();
    if(_accum.isFull())
    {
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
const FrameBox& AbstractDetector::frame(bool binarize)
{
    if(binarize)    cvwrap::otsuThreshold(_frame.data().asCvMat());
    return _frame;
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
    cvwrap::otsuThreshold(_frame.data().asCvMat());
    detector_hf::findTargets(_frame.data(),
                             _artifacts.data(),
                             _minSquare,
                             true,
                             _maxOblong);
    _artifacts.setTimeMarker(_frame.timeMarker());
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
