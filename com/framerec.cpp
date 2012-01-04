#include "framerec.h"
/////////////////////////////////////////////////////////////////////////////////////
FrameReceiver::FrameReceiver(QSettings *s,
                             Strob *strob,
                             FrameBox *f0,
                             FrameBox *f1) :
    _settings(s),
    _strob(strob),
    _frame0(f0),
    _frame1(f1),
    _sharedMem(new SharedMem(s)),
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
            if(_frame0->lock().tryLockForWrite(_timeout))
            {
                _sharedMem->readFrame(_frame0);
                this->checkFrameSize(_frame0->data().header().width(),
                                     _frame0->data().header().height());
                this->fastProc(_frame0->data());
                _frame0->lock().unlock();
                emit frame0Ready(_frame0); //to Gui
            }
            if(_frame1->lock().tryLockForWrite(_timeout))
            {
                _sharedMem->readFrame(_frame1);
                _frame1->lock().unlock();
                emit frame1Ready(_frame1,   //to StarDetector
                                 _strob->geometry().center().x(),
                                 _strob->geometry().center().y());
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void FrameReceiver::fastProc(Frame &f)
{
    _strob->makeTracking(&f);
}
/////////////////////////////////////////////////////////////////////////////////////
void FrameReceiver::checkFrameSize(const int width,
                                   const int height)
{
    QSize s(width, height);
    if(s != _bufSize)
    {
        _bufSize = s;
        emit frameSizeChanged(width, height);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
