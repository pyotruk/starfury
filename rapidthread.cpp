#include "rapidthread.h"
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::RapidThread(const int frameHeaderSize,
                         const int frameWidth,
                         const int frameHeight,
                         QSettings *settings)
{
    FFrame = new uchar [frameWidth * frameHeight];
    FSharedMem = new SharedMem(frameHeaderSize,
                               frameWidth,
                               frameHeight,
                               settings);
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
            FSharedMem->getData(FFrame);
            emit frameReceived(FFrame, 640, 480);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
