#include "readsharedmem.h"
/////////////////////////////////////////////////////////////////////////////////////
SharedMem::SharedMem(LPCTSTR fileMapId,
                     LPCTSTR eventId,
                     LPCTSTR mutexId,
                     int frameHeaderSize,
                     int frameWidth,
                     int frameHeight)
{
      FFrameSizes.headerSize = frameHeaderSize;
      FFrameSizes.width = frameWidth;
      FFrameSizes.height = frameHeight;
      FhMappedFile = CreateFileMapping(INVALID_HANDLE_VALUE,
                                       NULL,
                                       PAGE_READWRITE,
                                       0,
                                       FFrameSizes.width * FFrameSizes.height + FFrameSizes.headerSize,
                                       fileMapId);
      if(FhMappedFile == 0)
          qDebug() << "CreateFileMapping error";

      FpSharedBuf = MapViewOfFile(FhMappedFile,
                                  FILE_MAP_ALL_ACCESS,
                                  0,
                                  0,
                                  0);
      if(FpSharedBuf == NULL)
          qDebug() << "MapViewOfFile error";

      FhEvent = CreateEvent(NULL,
                            true,
                            false,
                            eventId);
      if(FhEvent == 0)
          qDebug() << "CreateEvent error";

      FhMutex = CreateMutex(NULL,
                            false,
                            mutexId);
      if(FhMutex == 0)
          qDebug() << "CreateMutex error";      

      FpFrame = new uchar [FFrameSizes.width * FFrameSizes.height];
      start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
SharedMem::~SharedMem()
{
    exit(); //a-la suspend for TThread
    terminate();
    UnmapViewOfFile(FpSharedBuf);
    CloseHandle(FhMappedFile);
    CloseHandle(FhEvent);
    CloseHandle(FhMutex);
    delete FpFrame;
}
///////////////////////////////////////////////////////////////////////////////////////
void SharedMem::ReadBuf()
{
    uchar *pFrameEnd = FpFrame + FFrameSizes.width * FFrameSizes.height;
    for(uchar *ff = FpFrame,
              *pSharedBuf = (uchar*)FpSharedBuf + FFrameSizes.headerSize;
        ff < pFrameEnd;
        ff++,
        pSharedBuf++)
        *ff = *pSharedBuf;
}
/////////////////////////////////////////////////////////////////////////////////////
void SharedMem::run()
{
    DWORD waitRes;
    while(isRunning())
    {
        waitRes = WaitForSingleObject(FhEvent, 500);
        if(waitRes == 0)
        {
            waitRes = WaitForSingleObject(FhMutex, 20);
            if(waitRes == 0)
            {
                ReadBuf();
                ReleaseMutex(FhMutex);
                ResetEvent(FhEvent);
                emit DrawFrameSignal((void*)FpFrame,
                                     FFrameSizes.width,
                                     FFrameSizes.height); //сигнал на отрисовку картинки
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////
