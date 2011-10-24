#ifndef READSHAREDMEM_H
#define READSHAREDMEM_H

#include <QDebug>
#include "windows.h"
#include "qt_windows.h"
#include <QSettings>

//default values
#define DEFAULT_SHARED_MEM_ID   "{TVA2-MemoryID}"
#define DEFAULT_SHARED_EVENT_ID "{TVA2-EventID}"
#define DEFAULT_SHARED_MUTEX_ID "{TVA2-MutexID}"
//setting keys
#define SKEY_SHARED_MEM_ID   "/SharedMem/FileMapID"
#define SKEY_SHARED_EVENT_ID "/SharedMem/EventID"
#define SKEY_SHARED_MUTEX_ID "/SharedMem/MutexID"

//////////////////////////////////////////////////////////////////////////////////////
struct FrameHeader
{
  qint64  timeID;
  qint64  timeFreq;
  quint32 dataSize;
  quint32 width;
  quint32 height;
  quint32 frameID;
};
//////////////////////////////////////////////////////////////////////////////////////
struct SharedSettings
{
    QString   fileMapId;
    QString   eventId;
    QString   mutexId;
    QSettings *settings;
};
//////////////////////////////////////////////////////////////////////////////////////
class SharedMem
{
public:
    explicit SharedMem(QSettings *settings = 0);
    ~SharedMem();
    bool waitForData();
    void getHeader(FrameHeader *header);
    void getData(const FrameHeader &header,
                 void  *data);
private:
    SharedSettings FSharedSettings;
    void           *FSharedBuf;
    HANDLE         FhMappedFile;
    HANDLE         FhEvent;
    HANDLE         FhMutex;
    void loadSettings(QSettings *settings);
    void saveSettings(QSettings *settings);
};

#endif // READSHAREDMEM_H
