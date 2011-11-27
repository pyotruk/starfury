#include "framerec.h"
/////////////////////////////////////////////////////////////////////////////////////
FrameReceiver::FrameReceiver(QSettings *s,
                             Frame *f) :
    _settings(s),
    _frame(f),
    _sharedMem(new SharedMem(_settings)),
    _stopped(false)
{
    this->moveToThread(this);
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
FrameReceiver::~FrameReceiver()
{
    this->stop();
    if(!this->wait(_termTimeout))   this->terminate();
    delete _sharedMem;
}
/////////////////////////////////////////////////////////////////////////////////////
void FrameReceiver::run()
{
    while(!_stopped)
    {
        if(_sharedMem->waitForData())
        {
            if(_frame->lock().tryLockForWrite(_timeout))
            {
                _sharedMem->readFrame(_frame);
                _frame->lock().unlock();
            }
            emit frameReady(_frame);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void FrameReceiver::stop()
{
    _stopped = true;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
