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
    quint32 frameID;
    quint32 dataSize;
    quint32 depth;
    quint32 width;
    quint32 height;
    quint32 plug;
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
    void readHeader(FrameHeader *header); //сначала прочитать заголовок
    void readData(void *data);            //потом прочитать данные (передать указатель, куда функция скопирует данные)
private:
    SharedSettings _sharedSettings;
    FrameHeader    _header;
    void           *_sharedBuf;
    HANDLE         _mappedFile;
    HANDLE         _event;
    HANDLE         _mutex;
    void loadSettings(QSettings *settings);
    void saveSettings(QSettings *settings);
};

#endif // READSHAREDMEM_H
