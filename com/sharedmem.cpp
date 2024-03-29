#include "sharedmem.h"
/////////////////////////////////////////////////////////////////////////////////////
SharedMem::SharedMem(QSettings *s)
{
      _sharedSettings.settings = s;
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
void SharedMem::loadSettings(QSettings *s)
{
    _sharedSettings.fileMapId = s->value(__skeySharedFileMapID, __defaultSharedFileMapID).toString();
    _sharedSettings.eventId = s->value(__skeySharedEventID, __defaultSharedEventID).toString();
    _sharedSettings.mutexId = s->value(__skeySharedMutexID, __defaultSharedMutexID).toString();
}
///////////////////////////////////////////////////////////////////////////////////////
void SharedMem::saveSettings(QSettings *s)
{
    s->setValue(__skeySharedFileMapID, _sharedSettings.fileMapId);
    s->setValue(__skeySharedEventID, _sharedSettings.eventId);
    s->setValue(__skeySharedMutexID, _sharedSettings.mutexId);
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
void SharedMem::readFrame(FrameBox *frame)
{
    uchar* buf = (uchar*)_sharedBuf;
    const void *src = (void*)buf;
    DataHeader header;
    void *dst = &header;
    memcpy(dst, src, sizeof(DataHeader));
    buf += sizeof(DataHeader);
    void *data = (void*)buf;
    frame->data().copyFromRawData(data,
                                  header.width,
                                  header.height);
    QDateTime t;
    timeutils::winfiletime2qdatetime(header.timeID, t);
    frame->setTimeMarker(t);

    ReleaseMutex(_mutex);
    ResetEvent(_event);
}
//////////////////////////////////////////////////////////////////////////////////////
