#include "rapidthread.h"
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::RapidThread(QSettings *settings) :
    _doubleBuf(new DoubleBuffer()),
    _sharedMem(new SharedMem(settings))
{
    start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::~RapidThread()
{
    exit();
    terminate();
    delete _sharedMem;
    delete _doubleBuf;
}
/////////////////////////////////////////////////////////////////////////////////////
void RapidThread::run()
{
    while(isRunning())
    {
        if(_sharedMem->waitForData())
        {
            _sharedMem->readFrame(*(_doubleBuf->rapidBuf()));
            emit frame4Strob(_doubleBuf->rapidBuf());
            if(! _doubleBuf->slowBuf()->isLocked())
            {
                _doubleBuf->switchBuffers();
                _doubleBuf->slowBuf()->lock();
                emit frame4Gui(_doubleBuf->slowBuf());
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
DoubleBuffer *RapidThread::doubleBuf()
{
    return _doubleBuf;
}
/////////////////////////////////////////////////////////////////////////////////////
