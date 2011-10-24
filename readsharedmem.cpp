#include "readsharedmem.h"
/////////////////////////////////////////////////////////////////////////////////////
SharedMem::SharedMem(QSettings *settings)
{
      FSharedSettings.settings = settings;
      loadSettings(FSharedSettings.settings);

      FhMappedFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,
                                     false,
                                     (LPCTSTR)FSharedSettings.fileMapId.data());
      if(FhMappedFile == 0)  qDebug() << "OpenFileMapping error";

      FSharedBuf = MapViewOfFile(FhMappedFile,
                                 FILE_MAP_ALL_ACCESS,
                                 0,
                                 0,
                                 0);
      if(FSharedBuf == NULL)  qDebug() << "MapViewOfFile error";

      FhEvent = OpenEvent(SYNCHRONIZE,
                          false,
                          (LPCTSTR)FSharedSettings.eventId.data());
      if(FhEvent == 0)  qDebug() << "OpenEvent error";

      FhMutex = OpenMutex(SYNCHRONIZE,
                          false,
                          (LPCTSTR)FSharedSettings.mutexId.data());
      if(FhMutex == 0)  qDebug() << "OpenMutex error";
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
void SharedMem::getData(const FrameHeader &header,
                        void  *data)
{
    uchar *beginBuf = (uchar*)FSharedBuf + sizeof(FrameHeader);
    memcpy(data, (void*)beginBuf, header.width * header.height);
    ReleaseMutex(FhMutex);
    ResetEvent(FhEvent);
}
//////////////////////////////////////////////////////////////////////////////////////
void SharedMem::getHeader(FrameHeader *header)
{
    uchar *beginBuf = (uchar*)FSharedBuf;
    memcpy((void*)header, (void*)beginBuf, sizeof(FrameHeader));
}
//////////////////////////////////////////////////////////////////////////////////////
