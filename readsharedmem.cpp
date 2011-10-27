#include "readsharedmem.h"
/////////////////////////////////////////////////////////////////////////////////////
SharedMem::SharedMem(QSettings *settings)
{
      _sharedSettings.settings = settings;
      loadSettings(_sharedSettings.settings);

      _mappedFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,
                                    false,
                                    (LPCTSTR)_sharedSettings.fileMapId.data());
      if(_mappedFile == 0)  qDebug() << "OpenFileMapping error";

      _sharedBuf = MapViewOfFile(_mappedFile,
                                 FILE_MAP_ALL_ACCESS,
                                 0,
                                 0,
                                 0);
      if(_sharedBuf == NULL)  qDebug() << "MapViewOfFile error";

      _event = OpenEvent(SYNCHRONIZE,
                         false,
                         (LPCTSTR)_sharedSettings.eventId.data());
      if(_event == 0)  qDebug() << "OpenEvent error";

      _mutex = OpenMutex(SYNCHRONIZE,
                         false,
                         (LPCTSTR)_sharedSettings.mutexId.data());
      if(_mutex == 0)  qDebug() << "OpenMutex error";
}
/////////////////////////////////////////////////////////////////////////////////////
SharedMem::~SharedMem()
{
    UnmapViewOfFile(_sharedBuf);
    CloseHandle(_mappedFile);
    CloseHandle(_event);
    CloseHandle(_mutex);
    saveSettings(_sharedSettings.settings);
}
///////////////////////////////////////////////////////////////////////////////////////
void SharedMem::loadSettings(QSettings *settings)
{
    if(settings == 0)
    {
        _sharedSettings.fileMapId = DEFAULT_SHARED_MEM_ID;
        _sharedSettings.eventId = DEFAULT_SHARED_EVENT_ID;
        _sharedSettings.mutexId = DEFAULT_SHARED_MUTEX_ID;
    }
    else
    {
        _sharedSettings.fileMapId = settings->value(SKEY_SHARED_MEM_ID, DEFAULT_SHARED_MEM_ID).toString();
        _sharedSettings.eventId = settings->value(SKEY_SHARED_EVENT_ID, DEFAULT_SHARED_EVENT_ID).toString();
        _sharedSettings.mutexId = settings->value(SKEY_SHARED_MUTEX_ID, DEFAULT_SHARED_MUTEX_ID).toString();
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void SharedMem::saveSettings(QSettings *settings)
{
    if(settings != 0)
    {
        settings->setValue(SKEY_SHARED_MEM_ID, _sharedSettings.fileMapId);
        settings->setValue(SKEY_SHARED_EVENT_ID, _sharedSettings.eventId);
        settings->setValue(SKEY_SHARED_MUTEX_ID, _sharedSettings.mutexId);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
bool SharedMem::waitForData()
{
    bool res = false;
    DWORD waitRes = WaitForSingleObject(_event, 500);
    if(waitRes == 0)
    {
        waitRes = WaitForSingleObject(_mutex, 20);
        if(waitRes == 0)  res = true;
    }
    return res;
}
//////////////////////////////////////////////////////////////////////////////////////
void SharedMem::readData(void  *data)
{
    uchar *beginBuf = (uchar*)_sharedBuf + sizeof(FrameHeader);
    memcpy(data, (void*)beginBuf, _header.dataSize);
    ReleaseMutex(_mutex);
    ResetEvent(_event);
}
//////////////////////////////////////////////////////////////////////////////////////
void SharedMem::readHeader(FrameHeader *header)
{
    uchar *beginBuf = (uchar*)_sharedBuf;
    memcpy((void*)header, (void*)beginBuf, sizeof(FrameHeader));
    _header = *header;
}
//////////////////////////////////////////////////////////////////////////////////////
