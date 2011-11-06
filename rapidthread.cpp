#include "rapidthread.h"
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::RapidThread(QSettings *settings) :
    _frame(new Frame),
    _strob(new Strob(settings, this)),
    _mutex(new QMutex)
{
    this->moveToThread(this);
    qDebug() << "rapidThread->_strob thread:" << _strob->thread();
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::~RapidThread()
{
    this->quit();
    this->terminate();
    delete _mutex;
    delete _strob;
    delete _frame;
}
/////////////////////////////////////////////////////////////////////////////////////
void RapidThread::frameIn(Frame *frame,
                          QMutex *mutex)
{
    if(mutex->tryLock(_timeout))
    {
        _strob->makeTracking(frame);
        if(this->_mutex->tryLock(_timeout))
        {
            *_frame = *frame;
            this->_mutex->unlock();
        }
        mutex->unlock();
        emit frameOut1(_frame, this->_mutex);
        emit frameOut2(_frame, this->_mutex);
        emit frameOut3(_frame, this->_mutex);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
Strob *RapidThread::strob()
{
    return _strob;
}
/////////////////////////////////////////////////////////////////////////////////////
