#include "snudpsrv.h"
/////////////////////////////////////////////////////////////////////////////////////
SnUdpSrv::SnUdpSrv(QSettings *s) :
    _settings(s)
{
    loadSettings(_settings);
    qDebug() << "QUdpSocket thread: " << _socket.thread();
    connect(&_socket, SIGNAL(readyRead()),
            this, SLOT(read()),
            Qt::AutoConnection);
    if(! _socket.bind(QHostAddress::LocalHost, _port))
        qDebug() << "bind failed";
}
/////////////////////////////////////////////////////////////////////////////////////
SnUdpSrv::~SnUdpSrv()
{
    saveSettings(_settings);
}
/////////////////////////////////////////////////////////////////////////////////////
void SnUdpSrv::loadSettings(QSettings *s)
{
    _port = s->value(__skeyUdpServerPort, _defaultPort).toInt();
}
/////////////////////////////////////////////////////////////////////////////////////
void SnUdpSrv::saveSettings(QSettings *s)
{
    s->setValue(__skeyUdpServerPort, _port);
}
/////////////////////////////////////////////////////////////////////////////////////
void SnUdpSrv::read()
{
    if(_socket.pendingDatagramSize() == sizeof(TelescopeStatus))
    {
        if(_telescope.lock().tryLockForWrite(_timeout))
        {
            _socket.readDatagram((char*)(&(_telescope.data())),
                                  sizeof(TelescopeStatus));
            _telescope.lock().unlock();
            emit telescopeStatusReady(&_telescope);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////