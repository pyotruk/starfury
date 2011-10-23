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
    void InitFace(int strobSize);
private:
    Ui::MainWindow *ui;
    QImage *FImg;
private slots:
    void paintEvent(QPaintEvent *);
    void DrawFrameSlot(void   *pFrame,
                       int frameWidth,
                       int frameHeight);
signals:
    void mousePressEvent(QMouseEvent *);
    void StrobSizeChangedSignal(int pos);
};

#endif // MAINWINDOW_H
