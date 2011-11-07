#ifndef FRAMEREC_H
#define FRAMEREC_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QMutex>
#include <QSettings>
#include "sharedmem.h"
#include "frame.h"
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
class FrameReceiver : public QThread
{
    Q_OBJECT
public:
    explicit FrameReceiver(QSettings *settings = 0);
    ~FrameReceiver();
protected:
    void run();
private:
    static const int _timeout = 20;
    QSettings *_settings;
    SharedMem *_sharedMem;
    Frame     *_frame;
    QMutex    *_mutex;
signals:
    void frameReady(Frame*, QMutex*);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // FRAMERECEIVER_H