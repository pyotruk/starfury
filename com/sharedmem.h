#ifndef SHAREDMEM_H
#define SHAREDMEM_H
//////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include "windows.h"
#include "qt_windows.h"
#include <QSettings>
#include <QString>
//////////////////////////////////////////////////////////////////////////////////////
#include "boxes/frame.h"
//////////////////////////////////////////////////////////////////////////////////////
//default values
static const QString __defaultSharedFileMapID("{TVA2-MemoryID}");
static const QString __defaultSharedEventID("{TVA2-EventID}");
static const QString __defaultSharedMutexID("{TVA2-MutexID}");
//setting keys
static const QString __skeySharedFileMapID("/SharedMem/FileMapID");
static const QString __skeySharedEventID("/SharedMem/EventID");
static const QString __skeySharedMutexID("/SharedMem/MutexID");
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
    explicit SharedMem(QSettings*);
    ~SharedMem();
    bool waitForData();
    void readFrame(Frame *frame);
private:
    SharedMem(const SharedMem&) {}
    SharedMem& operator =(const SharedMem&) {return *this;}
    SharedSettings _sharedSettings;
    void           *_sharedBuf;
    HANDLE         _mappedFile;
    HANDLE         _event;
    HANDLE         _mutex;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
};

#endif // SHAREDMEM_H
