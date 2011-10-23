#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>


typedef unsigned char uchar;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initFace(int strobSize,
                  int trackTresh);
private:
    Ui::MainWindow *ui;
    QImage *FImg;
private slots:
    void paintEvent(QPaintEvent *);
    void drawFrame(void *pFrame,
                   int  frameWidth,
                   int  frameHeight);
    void updateFace();
signals:
    void mousePressEvent(QMouseEvent *);
    void changeStrobSize(int pos);
    void changeTrackingThreshold(int pos);
};

#endif // MAINWINDOW_H
