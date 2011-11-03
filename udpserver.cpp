#include "udpserver.h"
/////////////////////////////////////////////////////////////////////////////////////
UdpServer::UdpServer(QSettings *settings) :
    _settings(settings),
    _socket(new QUdpSocket(this))
{
    loadSettings(_settings);
//    connect(_socket, SIGNAL(readyRead()),
//            this, SLOT(read()),
//            Qt::DirectConnection);
    qDebug() << "UdpServer thread: " << this->thread();
    qDebug() << "QUdpSocket thread: " << _socket->thread();
    if(_socket->bind(QHostAddress::LocalHost, _port))   this->start(QThread::NormalPriority);
    else    qDebug() << "bind failed";
}
/////////////////////////////////////////////////////////////////////////////////////
UdpServer::~UdpServer()
{
    this->terminate();
    saveSettings(_settings);
    delete _socket;
}
/////////////////////////////////////////////////////////////////////////////////////
void UdpServer::loadSettings(QSettings *settings)
{
    if(settings == 0)
    {
       _port = _defaultPort;
    }
    else
    {
       _port = settings->value(SKEY_UDPSERVER_PORT, _defaultPort).toInt();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void UdpServer::saveSettings(QSettings *settings)
{
    if(settings != 0)
    {
        settings->setValue(SKEY_UDPSERVER_PORT, _port);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void UdpServer::run()
{
    if(this->isRunning())
    {
        if(_socket->hasPendingDatagrams())
        {
            if(_socket->pendingDatagramSize() == sizeof(PackFromSN))
            {
                _socket->readDatagram((char*)&_pack, sizeof(PackFromSN));
                qDebug() << "udpServer: forwardData signal will be emit";
                emit forwardData(&_pack);
                qDebug() << "udpServer: forwardData signal has been emitted";
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
