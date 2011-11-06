#include "snudpsrv.h"
/////////////////////////////////////////////////////////////////////////////////////
SnUdpSrv::SnUdpSrv(QSettings *settings) :
    _settings(settings),
    _tvector(new TelescopeVector),
    _mutex(new QMutex)
{
    this->moveToThread(this);
    loadSettings(_settings);
    _socket = new QUdpSocket(this);
    //_socket->moveToThread(this);
    qDebug() << "QUdpSocket thread: " << _socket->thread();
//    connect(_socket, SIGNAL(readyRead()),
//            this, SLOT(read()),
//            Qt::AutoConnection);
    if(! _socket->bind(QHostAddress::LocalHost, _port))
        qDebug() << "bind failed";

    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
SnUdpSrv::~SnUdpSrv()
{
    this->quit();
    this->terminate();
    saveSettings(_settings);
    delete _socket;
    delete _mutex;
    delete _tvector;
}
/////////////////////////////////////////////////////////////////////////////////////
void SnUdpSrv::loadSettings(QSettings *settings)
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
void SnUdpSrv::saveSettings(QSettings *settings)
{
    if(settings != 0)
    {
        settings->setValue(SKEY_UDPSERVER_PORT, _port);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
//void SnUdpSrv::run()
//{

//}
/////////////////////////////////////////////////////////////////////////////////////
void SnUdpSrv::run()
{
    if(_socket->hasPendingDatagrams())
    {
    if(_socket->pendingDatagramSize() == sizeof(TelescopeVector))
    {
        if(_mutex->tryLock(_timeout))
        {
            _socket->readDatagram((char*)_tvector, sizeof(TelescopeVector));
            _mutex->unlock();
        }
        emit dataReady(_tvector, _mutex);
    }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
