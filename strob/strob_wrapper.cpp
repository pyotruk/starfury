#include "strob_wrapper.h"
/////////////////////////////////////////////////////////////////////////////////////
StrobWrapper::StrobWrapper(QSettings *s,
                           LogFile *log,
                           ArtifactBox *targets) :
    _log(log),
    _targets(targets),
    _strob(new Strob(s))
{
    this->moveToThread(this);
    _strob->moveToThread(this);
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
StrobWrapper::~StrobWrapper()
{
    this->quit();
    this->wait();
    delete _strob;
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobWrapper::inputFrame(FrameBox *f)
{
    f->lock().lockForWrite();
    Strob::RETURN_VALUES ret = _strob->proc(f->data());
    f->lock().unlock();
    emit frameReady(f);
    emit sendPhotometry(timeutils::msecFromDayBegin() / 1000,
                        _strob->signalMean(),
                        _strob->foneMean());

    switch(ret)
    {
    case Strob::OK:
        //qDebug() << "StrobWrapper says: OK";
        break;
    case Strob::LOCKED:
        //qDebug() << "StrobWrapper says: LOCKED";
        break;
    case Strob::BAD_GEOMETRY:
        qDebug() << "StrobWrapper says: BAD_GEOMETRY";
        break;
    }

    if(ret < 0)    return;

    if(_targets->lock().tryLockForWrite(_timeout))
    {
        _targets->data().clear();
        Artifact a;
        _strob->toArtifact(a);
        _targets->data().push_back(a);
        _targets->lock().unlock();
        emit freshTargets(_targets);

        //qDebug() << "StrobWrapper says: emit freshTargets(), target = " << a;
    }

    _log->write(QString::number(_strob->geometry().dx()) + " " +
                QString::number(_strob->geometry().dy()));
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobWrapper::targetsDetected()
{
    _targets->lock().lockForRead();
    if(!_targets->data().empty())
    {
        Artifact a = _targets->data().front();
        _strob->setCenter(a.center());
    }
    _targets->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobWrapper::setPos(QMouseEvent *e)
{
    _strob->setCenter(e->pos());
    qDebug() << "StrobWrapper says: QMouseEvent handler" << "\n"
             << "   mouse pos = " << e->pos();
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobWrapper::setSide(const int s)
{
    _strob->setSide(s);
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobWrapper::setThreshold(const int t)
{
    _strob->setThreshold((double)t);
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobWrapper::setVelocity(const double vx,
                               const double vy)
{
    //qDebug() << "StrobWrapper says: screen velocity = " << vx << "    " << vy;
    _strob->setVelocity(QPointF(vx, vy));
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
