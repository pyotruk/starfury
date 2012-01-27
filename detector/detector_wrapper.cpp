#include "detector/detector_wrapper.h"
/////////////////////////////////////////////////////////////////////////////////////
DetectorWrapper::DetectorWrapper(QSettings *s,
                                 FrameBox *shared_Frame,
                                 ArtifactBox *shared_Stars,
                                 ArtifactBox *shared_Targets)
    : _shared_Frame(shared_Frame),
      _shared_Stars(shared_Stars),
      _shared_Targets(shared_Targets),
      _targetDetector(new TargetDetector(s)),
      _starDetector(new StarDetector(s)),
      _binEnabled(false),
      _cache_AccumCap(0),
      _cache_Vel(0, 0)
{
    this->moveToThread(this);
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
DetectorWrapper::~DetectorWrapper()
{
    this->quit();
    this->wait();
    delete _starDetector;
    delete _targetDetector;
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWrapper::loadSettings(QSettings *s)
{
    _mode = (MODE)s->value(__skeyDetectorMode, _defaultMode).toInt();
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWrapper::saveSettings(QSettings *s)
{
    s->setValue(__skeyDetectorMode, _mode);
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWrapper::setMode(int m)
{
    _mode = (MODE)m;
    _targetDetector->clearAccum();
    _starDetector->clearAccum();
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWrapper::inputScreenVelocity(const double vx,
                                          const double vy)
{
    _cache_Vel = QVector2D(vx, vy);
    qDebug() << "DetectorWrapper: inputScreenVelocity(): vel = " << _cache_Vel;
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWrapper::inputFrame(FrameBox *f)
{
    switch(_mode)
    {
    case TARGET_DETECTION:
        this->do_Target_Detection(f);
        break;
    case STAR_DETECTION:
        this->do_Star_Detection(f);
        break;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWrapper::do_Target_Detection(FrameBox *f)
{
    if(_targetDetector->mutex().tryLock(_timeout))
    {
        _shared_Frame->lock().lockForWrite();
        _shared_Frame = _targetDetector->frame();
        _shared_Frame->lock().unlock();
        emit sendFrame(_shared_Frame);

        if(_targetDetector->ready())
        {
            _shared_Targets->lock().lockForWrite();
            _shared_Targets = _targetDetector->artifacts();
            _shared_Targets->lock().unlock();
            emit targetsReady(_shared_Targets);
        }

        _targetDetector->setAccumCapacity(_cache_AccumCap);

        f->lock().lockForRead();
        _targetDetector->inputFrame(f);
        f->lock().unlock();

        _targetDetector->mutex().unlock();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWrapper::do_Star_Detection(FrameBox *f)
{
    if(_starDetector->mutex().tryLock(_timeout))
    {
        _shared_Frame->lock().lockForWrite();
        _shared_Frame = _starDetector->frame();
        _shared_Frame->lock().unlock();
        emit sendFrame(_shared_Frame);

        if(_starDetector->ready())
        {
            _shared_Stars->lock().lockForWrite();
            _shared_Stars = _starDetector->artifacts();
            _shared_Stars->lock().unlock();
            emit starsReady(_shared_Stars);
        }

        _starDetector->setAccumCapacity(_cache_AccumCap);
        _starDetector->setVelocity(_cache_Vel);

        f->lock().lockForRead();
        _starDetector->inputFrame(f);
        f->lock().unlock();

        _starDetector->mutex().unlock();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
