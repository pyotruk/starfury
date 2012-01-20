#ifndef IMAGE_WND_H
#define IMAGE_WND_H
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
    explicit ImageWindow(const QString &title,
                         const QPoint  &pos,
                         QWidget *parent = 0);
    ~ImageWindow() {}
public slots:
    void inputFrame(FrameBox*);
protected:
    QImage  _img;
    virtual void drawAll() {}
private:
    ImageWindow(const ImageWindow&) {}
    ImageWindow& operator =(const ImageWindow&) {return *this;}
    QSize  _imgSize;
    QPoint _pos;
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
#endif // IMAGE_WND_H