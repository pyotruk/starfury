#include <QtGui/QApplication>
#include <QString>
#include <QObject>
#include <QSettings>
#include <QDebug>

#include "mainwindow.h"
#include "rapidthread.h"
#include "readsharedmem.h"
#include "frame.h"
#include "angmeas.h"
#include "starcat.h"
#include "udpserver.h"

#define FRAME_HEADER_SIZE 32


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QSettings settings("NIIPP", "astrobot");

    //Strob strob(&settings);
    RapidThread rapidThread(&settings);
    //AngMeas angMeas(&settings);
    //StarCat starCat(&settings);
    //UdpServer udpServer(&settings);

    qDebug() << "QApplication a thread: " << a.thread();
    qDebug() << "MainWidow w thread: " << w.thread();
    //qDebug() << "strob thread: " << strob.thread();
    qDebug() << "rapidThread thread: " << rapidThread.thread();
    //qDebug() << "angMeas thread: " << angMeas.thread();
    //qDebug() << "starCat thread: " << starCat.thread();
    //qDebug() << "udpServer thread: " << udpServer.thread();

    //form init
    w.initFace(rapidThread.strob()->geometry().innerSide(),
               (int)(rapidThread.strob()->threshold()));

    //object <--> object connections
//    QObject::connect(&rapidThread, SIGNAL(frame4Strob(Frame*)),
//                     &strob, SLOT(makeTracking(Frame*)),
//                     Qt::DirectConnection);
//    QObject::connect(&rapidThread, SIGNAL(frame4AngMeas(Frame*)),
//                     &angMeas, SLOT(inputFrame(Frame*)),
//                     Qt::DirectConnection);
//    QObject::connect(&udpServer, SIGNAL(forwardData(PackFromSN*)),
//                     &starCat, SLOT(inputDataFromSN(PackFromSN*)),
//                     Qt::DirectConnection);
    QObject::connect(&rapidThread, SIGNAL(frame4Gui(Frame*)),
                     &w, SLOT(drawFrame(Frame*)),
                     Qt::QueuedConnection);

    //gui --> object connections
    QObject::connect(&w, SIGNAL(unlockSlowBuf()),
                     rapidThread.doubleBuf(), SLOT(unlockSlowBuf()),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(mousePressEvent(QMouseEvent *)),
                     rapidThread.strob(), SLOT(clickTarget(QMouseEvent *)),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(changeStrobSize(int)),
                     &(rapidThread.strob()->geometry()), SLOT(setSide(int)),
                     Qt::QueuedConnection);
    QObject::connect(&w, SIGNAL(changeTrackingThreshold(int)),
                     rapidThread.strob(), SLOT(setThreshold(int)),
                     Qt::QueuedConnection);

    w.show();
    return a.exec();
}
