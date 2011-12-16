#ifndef SNUDPSRV_H
#define SNUDPSRV_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QtGlobal>
#include <QtNetwork/QUdpSocket>
#include <QSettings>
#include <QObject>
#include <QReadWriteLock>
#include <QDebug>
#include <QString>
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
static const QString __skeyUdpServerPort("/UdpServer/Port");
/////////////////////////////////////////////////////////////////////////////////////
struct TelescopeVector
{
    qint64 packID;
    qint64 timeUTC;
    double azAxis;
    double elAxis;
    double azVelAxis;
    double elVelAxis;
    double azOrient;
    double elOrient;
    double azHoriz;
    double elHoriz;
    double alpha;
    double delta;
    double LST;
    double latitude;
    double fieldWidth;
    double fieldHeight;
};
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
    TelescopeVector  _telescope;
    QReadWriteLock   _lock;
    quint16          _port;
    QUdpSocket       _socket;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
private slots:
    void read();
signals:
    void telescopeVectorReady(const TelescopeVector*,
                              QReadWriteLock*);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // SNUDPSRV_H
