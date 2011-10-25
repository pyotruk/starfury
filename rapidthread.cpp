#include "rapidthread.h"
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::RapidThread(QSettings *settings) :
    FFrameSize(DEFAULT_FRAME_WIDTH, DEFAULT_FRAME_HEIGHT),
    FDoubleBuf(new DoubleBuffer()),
    FSharedMem(new SharedMem(settings))
{
    start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::~RapidThread()
{
    exit();
    terminate();
    delete FSharedMem;
    delete FDoubleBuf;
}
/////////////////////////////////////////////////////////////////////////////////////
void RapidThread::run()
{
    while(isRunning())
    {
        if(FSharedMem->waitForData())
        {
            FSharedMem->getHeader(&FFrameHeader);
            FDoubleBuf->setSize(FFrameHeader.width * FFrameHeader.height);
            FSharedMem->getData(FFrameHeader, FDoubleBuf->getRapidBuf()->getData());
            emit frame4RapidThread(FDoubleBuf->getRapidBuf()->getData(),
                                   FFrameHeader.width,
                                   FFrameHeader.height);
            if(! FDoubleBuf->getSlowBuf()->isLocked())
            {
                FDoubleBuf->switchBuffers();
                FDoubleBuf->getSlowBuf()->lock();
                emit frame4SlowThread(FDoubleBuf->getSlowBuf()->getData(),
                                      FFrameHeader.width,
                                      FFrameHeader.height);
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
DoubleBuffer *RapidThread::getDoubleBuf()
{
    return FDoubleBuf;
}
/////////////////////////////////////////////////////////////////////////////////////
