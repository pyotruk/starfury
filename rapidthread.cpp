#include "rapidthread.h"
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::RapidThread(QSettings *settings) :
    _doubleBuf(new DoubleBuffer()),
    _sharedMem(new SharedMem(settings)),
    _strob(new Strob(settings, this))
{
    this->moveToThread(this);
    _doubleBuf->moveToThread(this);
    _sharedMem->moveToThread(this);
    //_strob->moveToThread(this);
    qDebug() << "rapidThread->_doubleBuf thread:" << _doubleBuf->thread();
    qDebug() << "rapidThread->_sharedMem thread:" << _sharedMem->thread();
    qDebug() << "rapidThread->_strob thread:" << _strob->thread();
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::~RapidThread()
{
    this->quit();
    this->terminate();
    delete _sharedMem;
    delete _doubleBuf;
    delete _strob;
}
/////////////////////////////////////////////////////////////////////////////////////
void RapidThread::run()
{
    while(this->isRunning())
    {
        if(_sharedMem->waitForData())
        {
            _sharedMem->readFrame(*(_doubleBuf->rapidBuf()));
            //emit frame4Strob(_doubleBuf->rapidBuf());
            _strob->makeTracking(_doubleBuf->rapidBuf());
            emit frame4AngMeas(_doubleBuf->rapidBuf());
            if(! _doubleBuf->slowBuf()->isLocked())
            {
                _doubleBuf->switchBuffers();
                _doubleBuf->slowBuf()->lock();
                emit frame4Gui(_doubleBuf->slowBuf());
            }
        }
    }
    //exec();
}
/////////////////////////////////////////////////////////////////////////////////////
DoubleBuffer *RapidThread::doubleBuf()
{
    return _doubleBuf;
}
/////////////////////////////////////////////////////////////////////////////////////
Strob *RapidThread::strob()
{
    return _strob;
}
/////////////////////////////////////////////////////////////////////////////////////
