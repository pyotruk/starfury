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

typedef unsigned char uchar;

//////////////////////////////////////////////////////////////////////////////////////
struct FrameSizes
{
    int headerSize;
    int width;
    int height;
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
    explicit SharedMem(int frameHeaderSize,
                       int frameWidth,
                       int frameHeight,
                       QSettings *settings = 0);
    ~SharedMem();
    bool waitForData();
    void getData(void *data);
private:
    SharedSettings FSharedSettings;
    void           *FSharedBuf;
    HANDLE         FhMappedFile;
    HANDLE         FhEvent;
    HANDLE         FhMutex;
    FrameSizes     FFrameSizes;
    void loadSettings(QSettings *settings);
    void saveSettings(QSettings *settings);
};

#endif // READSHAREDMEM_H
