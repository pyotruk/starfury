#ifndef RAPIDTHREAD_H
#define RAPIDTHREAD_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QMutex>
#include <QSettings>
#include <QSize>
#include <QTime>
#include <QDebug>
#include "frame.h"
#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
class RapidThread : public QThread
{
    Q_OBJECT
public:
    explicit RapidThread(QSettings *settings = 0);
    ~RapidThread();
    Strob *strob();
private:
    static const int _timeout = 20;
    Frame  *_frame0,
           *_frame1;
    QMutex *_mutex0;
    QMutex *_mutex1;
    Strob  *_strob;
    void urgentProcessing(Frame*);
private slots:
    void frameIn(Frame*, QMutex*);
signals:
    void frameOut0(Frame*, QMutex*, int xTarget, int yTarget);
    void frameOut1(Frame*, QMutex*);
    void frameOut2(Frame*, QMutex*);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // RAPIDTHREAD_H
