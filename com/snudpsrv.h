#ifndef SNUDPSRV_H
#define SNUDPSRV_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QtGlobal>
#include <QtNetwork/QUdpSocket>
#include <QSettings>
#include <QObject>
#include <QDebug>
#include <QString>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/telescope.h"
/////////////////////////////////////////////////////////////////////////////////////
static const QString __skeyUdpServerPort("/UdpServer/Port");
/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class SnUdpSrv : public QObject
{
    Q_OBJECT
public:
    explicit SnUdpSrv(QSettings*);
    ~SnUdpSrv();
private:
    SnUdpSrv(const SnUdpSrv&) {}
    SnUdpSrv& operator =(const SnUdpSrv&) {return *this;}
    static const quint16 _defaultPort = 4444;
    static const int  _timeout = 20;
    QSettings       *_settings;
    TelescopeBox     _telescope;
    quint16          _port;
    QUdpSocket       _socket;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
private slots:
    void read();
signals:
    void telescopeStatusReady(TelescopeBox*);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // SNUDPSRV_H
