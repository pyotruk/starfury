#ifndef LOGFILE_H
#define LOGFILE_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDateTime>
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////
#include "utils/timeutils.h"
/////////////////////////////////////////////////////////////////////////////////////
class LogFile
{
public:
    explicit LogFile(const QString &nameMask); //name format: time + nameMask + .log
    ~LogFile();
    void write(const QString&);
private:
    LogFile(const LogFile&) {}
    LogFile& operator =(const LogFile&) {return *this;}
    QFile        _file;
    QTextStream *_stream;
    void setName(const QString &nameMask);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // LOGFILE_H
