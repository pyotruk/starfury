#include "timeutils.h"
/////////////////////////////////////////////////////////////////////////////////////
void timeutils::winfiletime2qdatetime(const qint64 &fileTime,
                                      QDateTime    &qtTime)
{
    FILETIME ftime;
    SYSTEMTIME sysTime;
    qint32 *p = (qint32*)(&fileTime);
    ftime.dwLowDateTime = *p;
    ++p;
    ftime.dwHighDateTime = *p;
    FileTimeToSystemTime(&ftime, &sysTime);
    QDate date(sysTime.wYear,
               sysTime.wMonth,
               sysTime.wDay);
    QTime time(sysTime.wHour,
               sysTime.wMinute,
               sysTime.wSecond,
               sysTime.wMilliseconds);
    qtTime.setDate(date);
    qtTime.setTime(time);
}
/////////////////////////////////////////////////////////////////////////////////////
int timeutils::msecFromDayBegin()
{
    QTime t0(0, 0, 0, 0);
    int msec = t0.msecsTo(QTime::currentTime());
    return msec;
}
/////////////////////////////////////////////////////////////////////////////////////
int timeutils::msecFromDayBegin(QDateTime &t)
{
    QTime t0(0, 0, 0, 0);
    int msec = t0.msecsTo(t.time());
    return msec;
}
/////////////////////////////////////////////////////////////////////////////////////
bool timeutils::checkTimeDelay(const QDateTime &t1,
                               const QDateTime &t2,
                               const int maxDelay)
{
    int dt = qAbs(t1.time().msecsTo(t2.time()));
    if(dt > maxDelay)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
