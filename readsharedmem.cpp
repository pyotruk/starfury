#include "readsharedmem.h"
/////////////////////////////////////////////////////////////////////////////////////
SharedMem::SharedMem(int frameHeaderSize,
                     int frameWidth,
                     int frameHeight,
                     QSettings *settings)
{
      FSharedSettings.settings = settings;
      loadSettings(FSharedSettings.settings);
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

      FSharedBuf = MapViewOfFile(FhMappedFile,
                                 FILE_MAP_ALL_ACCESS,
                                 0,
                                 0,
                                 0);
      if(FSharedBuf == NULL)
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
}
/////////////////////////////////////////////////////////////////////////////////////
SharedMem::~SharedMem()
{
    UnmapViewOfFile(FSharedBuf);
    CloseHandle(FhMappedFile);
    CloseHandle(FhEvent);
    CloseHandle(FhMutex);
    saveSettings(FSharedSettings.settings);
}
///////////////////////////////////////////////////////////////////////////////////////
void SharedMem::loadSettings(QSettings *settings)
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
void SharedMem::saveSettings(QSettings *settings)
{
    if(settings != 0)
    {
        settings->setValue(SKEY_SHARED_MEM_ID, FSharedSettings.fileMapId);
        settings->setValue(SKEY_SHARED_EVENT_ID, FSharedSettings.eventId);
        settings->setValue(SKEY_SHARED_MUTEX_ID, FSharedSettings.mutexId);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
bool SharedMem::waitForData()
{
    bool res = false;
    DWORD waitRes = WaitForSingleObject(FhEvent, 500);
    if(waitRes == 0)
    {
        waitRes = WaitForSingleObject(FhMutex, 20);
        if(waitRes == 0)  res = true;
    }
    return res;
}
//////////////////////////////////////////////////////////////////////////////////////
void SharedMem::getData(void *data)
{
    uchar *sharedBuf = (uchar*)FSharedBuf + FFrameSizes.headerSize;
    memcpy(data, (void*)sharedBuf, FFrameSizes.width * FFrameSizes.height);
    ReleaseMutex(FhMutex);
    ResetEvent(FhEvent);
}
//////////////////////////////////////////////////////////////////////////////////////
