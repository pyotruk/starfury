#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initFace(const int strobSize,
                  const int trackTresh);
private:
    static const int _defaultImgWidth  = 640;
    static const int _defaultImgHeight = 480;
    Ui::MainWindow *ui;
    QImage *_img;
    void setImgSize(const QSize &frameSize);
    void adaptWindowSize(const QSize &imgSize);
private slots:
    void paintEvent(QPaintEvent *);
    void drawFrame(const void *frame,
                   const int  frameWidth,
                   const int  frameHeight);
    void updateFace();
signals:
    void mousePressEvent(QMouseEvent *);
    void changeStrobSize(const int pos);
    void changeTrackingThreshold(const int pos);
    void unlockSlowBuf(); //slowBuf unlocking (in rapidThread.doubleBuffer)
};

#endif // MAINWINDOW_H
