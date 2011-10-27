#include "rapidthread.h"
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::RapidThread(QSettings *settings) :
    _frameSize(_defaultFrameWidth, _defaultFrameHeight),
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
            _sharedMem->readHeader(&_frameHeader);
            _doubleBuf->setSize(_frameHeader.dataSize);
            _sharedMem->readData(_doubleBuf->getRapidBuf()->getData());
            emit frame4RapidThread(_doubleBuf->getRapidBuf()->getData(),
                                   _frameHeader.width,
                                   _frameHeader.height);
            if(! _doubleBuf->getSlowBuf()->isLocked())
            {
                _doubleBuf->switchBuffers();
                _doubleBuf->getSlowBuf()->lock();
                emit frame4SlowThread(_doubleBuf->getSlowBuf()->getData(),
                                      _frameHeader.width,
                                      _frameHeader.height);
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
DoubleBuffer *RapidThread::getDoubleBuf()
{
    return _doubleBuf;
}
/////////////////////////////////////////////////////////////////////////////////////
