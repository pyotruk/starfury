#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QMainWindow>
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////
#include "astrometry/astrometry.h"
#include "detector/detector.h"
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
                  const astrometry::METHOD,
                  const int accumCapacity,
                  const Detector::MODE);
protected:
    void closeEvent(QCloseEvent *);
private:
    static const double __deg2rad = 0.017453292519943295769236907684886;
    static const double __rad2deg = 57.295779513082320876798154814105;
    Ui::ControlWindow *ui;
private slots:
    void inputMeasureError(double errAlpha,  //rad
                           double errDelta); //rad
    void updateSliderLabels();
    void convertCheckBoxSignal(int);
    void setStarDetectionMode() {emit setDetectorMode((int)Detector::STAR_DETECTION);}
    void setTargetDetectionMode() {emit setDetectorMode((int)Detector::TARGET_DETECTION);}
signals:
    void changeStrobSize(const int);
    void changeTrackingThreshold(const int);
    void setSimtriMethod();
    void setFreevecMethod();
    void setAccumCapacity(const int);
    void setBinEnabled(bool);
    void setDetectorMode(int);
    void closed();
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // CONTROLWINDOW_H
