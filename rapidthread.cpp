#include "rapidthread.h"
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::RapidThread(QSettings *settings)
{
    FFrame = new uchar [DEFAULT_FRAME_WIDTH * DEFAULT_FRAME_HEIGHT];
    FSharedMem = new SharedMem(settings);
    start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::~RapidThread()
{
    exit();
    terminate();
    delete FSharedMem;
    delete FFrame;
}
/////////////////////////////////////////////////////////////////////////////////////
void RapidThread::run()
{
    while(isRunning())
    {
        if(FSharedMem->waitForData())
        {
            FSharedMem->getHeader(&FFrameHeader);
            checkFrameSize(FFrameHeader, FFrame);
            FSharedMem->getData(FFrameHeader, FFrame);
            emit frameReceived(FFrame, FFrameHeader.width, FFrameHeader.height);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void RapidThread::checkFrameSize(const FrameHeader &header,
                                 uchar *frame)
{
    if((header.width * header.height) != sizeof(*frame))
    {
        delete frame;
        frame = new uchar [header.width * header.height];
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
