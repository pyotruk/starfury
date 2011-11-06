#ifndef SHAREDMEM_H
#define SHAREDMEM_H
//////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include "windows.h"
#include "qt_windows.h"
#include <QSettings>
#include <frame.h>
//////////////////////////////////////////////////////////////////////////////////////
//default values
#define DEFAULT_SHARED_MEM_ID   "{TVA2-MemoryID}"
#define DEFAULT_SHARED_EVENT_ID "{TVA2-EventID}"
#define DEFAULT_SHARED_MUTEX_ID "{TVA2-MutexID}"
//setting keys
#define SKEY_SHARED_MEM_ID   "/SharedMem/FileMapID"
#define SKEY_SHARED_EVENT_ID "/SharedMem/EventID"
#define SKEY_SHARED_MUTEX_ID "/SharedMem/MutexID"
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
    void readFrame(Frame *frame);
private:
    SharedSettings _sharedSettings;
    void           *_sharedBuf;
    HANDLE         _mappedFile;
    HANDLE         _event;
    HANDLE         _mutex;
    void loadSettings(QSettings *settings);
    void saveSettings(QSettings *settings);
};

#endif // SHAREDMEM_H
