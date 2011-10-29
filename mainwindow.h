#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QMainWindow>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include "frame.h"
/////////////////////////////////////////////////////////////////////////////////////

namespace Ui {
    class MainWindow;
}
/////////////////////////////////////////////////////////////////////////////////////
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initFace(const int strobSize,
                  const int trackTresh);
private:
    Ui::MainWindow *ui;
    QImage _img;
    void adaptWindowSize(const QSize &imgSize);
private slots:
    void paintEvent(QPaintEvent *);
    void drawFrame(Frame *frame);
    void updateFace();
signals:
    void mousePressEvent(QMouseEvent *);
    void changeStrobSize(const int pos);
    void changeTrackingThreshold(const int pos);
    void unlockSlowBuf(); //slowBuf unlocking (in rapidThread.doubleBuffer)
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // MAINWINDOW_H
