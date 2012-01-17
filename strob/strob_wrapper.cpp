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
    _strob->makeTracking(f->data());
    f->lock().unlock();
    emit frameReady(f);
    emit sendPhotometry(timeutils::msecFromDayBegin() / 1000,
                        _strob->signalMean(),
                        _strob->foneMean());

    if(_strob->isLocked())    qDebug() << "StrobWrapper says: _strob is LOCKED";
    else                      qDebug() << "StrobWrapper says: _strob is _NOT_ LOCKED";

    if(_targets->lock().tryLockForWrite(_timeout))
    {
        _targets->data().clear();
        Artifact a((double)_strob->geometry().center().x(),
                   (double)_strob->geometry().center().y());
        _targets->data().push_back(a);
        _targets->lock().unlock();
        emit freshTargets(_targets);

        qDebug() << "StrobWrapper says: emit freshTargets(_targets)" << "\n"
                 << "   target = " << a;
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
        _strob->geometry().setCenter((int)a.center().x(),
                                     (int)a.center().y());
    }
    _targets->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobWrapper::setPos(QMouseEvent *e)
{
    /* fucking BUG here: sometimes e->pos() is not correctly (out of frame borders)
       BUG need to fix */

    _strob->geometry().setCenter(e->pos());
    qDebug() << "StrobWrapper says: QMouseEvent handler" << "\n"
             << "   mouse pos = " << e->pos();
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobWrapper::setPos(const int x,
                          const int y)
{
    _strob->geometry().setCenter(x, y);
}
/////////////////////////////////////////////////////////////////////////////////////
void StrobWrapper::setSide(const int s)
{
    _strob->geometry().setSide(s);
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
    qDebug() << "StrobWrapper says: screen velocity = " << vx << "    " << vy;
    _strob->geometry().setVelocity(QPointF(vx, vy));
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
