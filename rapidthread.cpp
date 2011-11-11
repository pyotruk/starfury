#include "rapidthread.h"
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::RapidThread(QSettings *settings) :
    _frame0(new Frame),
    _frame1(new Frame),
    _mutex0(new QMutex),
    _mutex1(new QMutex),
    _strob(new Strob(settings, this))
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
    delete _strob;
    delete _mutex0;
    delete _mutex1;
    delete _frame0;
    delete _frame1;
}
/////////////////////////////////////////////////////////////////////////////////////
void RapidThread::urgentProcessing(Frame *frame)
{
    _strob->makeTracking(frame);
}
/////////////////////////////////////////////////////////////////////////////////////
void RapidThread::frameIn(Frame *frame,
                          QMutex *mutex)
{
    if(mutex->tryLock(_timeout))
    {
        QTime time;
        time.start();
        urgentProcessing(frame);
        if(this->_mutex0->tryLock(_timeout))
        {
            *_frame0 = *frame;
            this->_mutex0->unlock();
            emit frameOut0(_frame0,
                           this->_mutex0,
                           _strob->geometry().center().x(),
                           _strob->geometry().center().y());
        }
        if(this->_mutex1->tryLock(_timeout))
        {
            *_frame1 = *frame;
            _mutex1->unlock();
            emit frameOut1(_frame1, this->_mutex1);
        }
        mutex->unlock();
        qDebug() << "framerec mutex unlocked t " << time.elapsed() << endl;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
Strob *RapidThread::strob()
{
    return _strob;
}
/////////////////////////////////////////////////////////////////////////////////////
