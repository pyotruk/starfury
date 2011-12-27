#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QMainWindow>
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////
#include "astrometry/astrometry.h"
/////////////////////////////////////////////////////////////////////////////////////
namespace Ui {
    class ControlWindow;
}
/////////////////////////////////////////////////////////////////////////////////////
class ControlWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ControlWindow(QWidget *parent = 0);
    ~ControlWindow();
    void initFace(const int strobSize,
                  const int trackTresh,
                  const astrometry::METHOD);
private:
    static const double __deg2rad = 0.017453292519943295769236907684886;
    static const double __rad2deg = 57.295779513082320876798154814105;
    Ui::ControlWindow *ui;
private slots:
    void inputMeasureError(double errAlpha,  //rad
                           double errDelta); //rad
    void updateFace();
signals:
    void changeStrobSize(const int);
    void changeTrackingThreshold(const int);
    void setSimtriMethod();
    void setFreevecMethod();
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // CONTROLWINDOW_H
