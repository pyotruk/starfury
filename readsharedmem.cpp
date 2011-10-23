#include "readsharedmem.h"
/////////////////////////////////////////////////////////////////////////////////////
SharedMem::SharedMem(int frameHeaderSize,
                     int frameWidth,
                     int frameHeight,
                     QSettings *settings)
{
      FSharedSettings.settings = settings;
      LoadSettings(FSharedSettings.settings);
      FFrameSizes.headerSize = frameHeaderSize;
      FFrameSizes.width = frameWidth;
      FFrameSizes.height = frameHeight;
      FhMappedFile = CreateFileMapping(INVALID_HANDLE_VALUE,
                                       NULL,
                                       PAGE_READWRITE,
                                       0,
                                       FFrameSizes.width * FFrameSizes.height + FFrameSizes.headerSize,
                                       (LPCTSTR)FSharedSettings.fileMapId.data());
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
                            (LPCTSTR)FSharedSettings.eventId.data());
      if(FhEvent == 0)
          qDebug() << "CreateEvent error";

      FhMutex = CreateMutex(NULL,
                            false,
                            (LPCTSTR)FSharedSettings.mutexId.data());
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
    SaveSettings(FSharedSettings.settings);
}
///////////////////////////////////////////////////////////////////////////////////////
void SharedMem::LoadSettings(QSettings *settings)
{
    if(settings == 0)
    {
        FSharedSettings.fileMapId = DEFAULT_SHARED_MEM_ID;
        FSharedSettings.eventId = DEFAULT_SHARED_EVENT_ID;
        FSharedSettings.mutexId = DEFAULT_SHARED_MUTEX_ID;
    }
    else
    {
        FSharedSettings.fileMapId = settings->value(SKEY_SHARED_MEM_ID, DEFAULT_SHARED_MEM_ID).toString();
        FSharedSettings.eventId = settings->value(SKEY_SHARED_EVENT_ID, DEFAULT_SHARED_EVENT_ID).toString();
        FSharedSettings.mutexId = settings->value(SKEY_SHARED_MUTEX_ID, DEFAULT_SHARED_MUTEX_ID).toString();
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void SharedMem::SaveSettings(QSettings *settings)
{
    if(settings != 0)
    {
        settings->setValue(SKEY_SHARED_MEM_ID, FSharedSettings.fileMapId);
        settings->setValue(SKEY_SHARED_EVENT_ID, FSharedSettings.eventId);
        settings->setValue(SKEY_SHARED_MUTEX_ID, FSharedSettings.mutexId);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
void SharedMem::ReadBuf()
{
    uchar *pSharedBuf = (uchar*)FpSharedBuf + FFrameSizes.headerSize;
    memcpy(FpFrame, pSharedBuf, FFrameSizes.width * FFrameSizes.height);
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
                emit StrobSignal((void*)FpFrame,
                                 FFrameSizes.width,
                                 FFrameSizes.height); //сигнал на обработку картинки
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////
