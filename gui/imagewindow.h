#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QMainWindow>
#include <QObject>
#include <QMouseEvent>
#include <QImage>
#include <QPainter>
#include <QPoint>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/frame.h"
/////////////////////////////////////////////////////////////////////////////////////
class ImageWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ImageWindow(QWidget *parent = 0) : QMainWindow(parent) {}
    ~ImageWindow() {}
public slots:
    void inputFrame(Frame*);
protected:
    QImage  _img;
    virtual void drawAll() {}
private:
    ImageWindow(const ImageWindow&) {}
    ImageWindow& operator =(const ImageWindow&) {return *this;}
    static const int _xPos = 200;
    static const int _yPos = 200;
    QSize  _imgSize;
    void checkImgSize();
private slots:
    void paintEvent(QPaintEvent *);
signals:
    void mousePressEvent(QMouseEvent *);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // IMAGEWINDOW_H