#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPoint>

typedef unsigned char uchar;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QImage *FImg;
private slots:
    void paintEvent(QPaintEvent *);
    void DrawFrameSlot(void   *pFrame,
                       int frameWidth,
                       int frameHeight);
};

#endif // MAINWINDOW_H
