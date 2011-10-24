#include "rapidthread.h"
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::RapidThread(QSettings *settings) :
    FFrameSize(DEFAULT_FRAME_WIDTH, DEFAULT_FRAME_HEIGHT)
{
    FFrame = new uchar [FFrameSize.width() * FFrameSize.height()];
    FSharedMem = new SharedMem(settings);
    start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
RapidThread::~RapidThread()
{
    exit();
    terminate();
    delete FSharedMem;
    delete [] FFrame;
}
/////////////////////////////////////////////////////////////////////////////////////
void RapidThread::run()
{
    while(isRunning())
    {
        if(FSharedMem->waitForData())
        {
            FSharedMem->getHeader(&FFrameHeader);

            //variable frame size
            if(!checkFrameSize(FFrameHeader, FFrameSize))
            {
                FFrameSize = QSize(FFrameHeader.width, FFrameHeader.height);
                delete [] FFrame;
                FFrame = new uchar [FFrameSize.width() * FFrameSize.height()];
            }

            FSharedMem->getData(FFrameHeader, FFrame);
            emit frameReceived(FFrame, FFrameHeader.width, FFrameHeader.height);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
bool RapidThread::checkFrameSize(const FrameHeader &header,
                                 const QSize       &frameSize)
{
    QSize newSize(header.width, header.height);
    if(frameSize == newSize)  return true;
    else  return false;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
