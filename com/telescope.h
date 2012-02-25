/*  ласс [Telescope] реализует приЄм по UDP
   вектора состо€ни€ телескопа от »митатора.
   Ёкземпл€р класса работает в контексте основного потока приложени€
   (условие дл€ работы QUdpSocket). */
#ifndef TELESCOPE_H
#define TELESCOPE_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <QtGlobal>
#include <QtNetwork/QUdpSocket>
#include <QVector>
#include <QDateTime>
#include <QSettings>
#include <QObject>
#include <QString>
/////////////////////////////////////////////////////////////////////////////////////
#include "utils/timeutils.h"
#include "boxes/databox.h"
/////////////////////////////////////////////////////////////////////////////////////
struct TelPos /* telescope position, вектор состо€ни€ телескопа */
{
    qint64 packID;
    qint64 timeUTC;     // windows file time
    double azAxis;      // [rad]
    double elAxis;      // [rad]
    double azVelAxis;   // [rad/sec]
    double elVelAxis;   // [rad/sec]
    double azOrient;    // [rad]
    double elOrient;    // [rad]
    double azHoriz;     // [rad]
    double elHoriz;     // [rad]
    double alpha;       // [rad]
    double delta;       // [rad]
    double LST;         // [rad] local sidereal time
    double latitude;    // [rad]
};
/////////////////////////////////////////////////////////////////////////////////////
typedef DataBox<TelPos> TelBox;
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
static const QString __skeyUdpServerPort("/UdpServer/Port");
/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class Telescope : public QObject
{
    Q_OBJECT
public:
    explicit Telescope(QSettings*);
    ~Telescope();
private:
    Telescope(const Telescope&) {}
    Telescope& operator =(const Telescope&) {return *this;}
    static const quint16 _defaultPort = 44444;
    static const int  _timeout = 20;
    QSettings *_settings;
    TelBox     _telescope;
    quint16    _port;
    QUdpSocket _socket;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
private slots:
    void read();
signals:
    void sendTelPos(TelBox*);  //излучаетс€ при приЄме вектора состо€ни€ телескопа
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // TELESCOPE_H
