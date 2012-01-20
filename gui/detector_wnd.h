#ifndef DETECTOR_WND_H
#define DETECTOR_WND_H
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#include "image_wnd.h"
#include "boxes/artifact.h"
#include "drawing.h"
#include "detector/detector.h"
/////////////////////////////////////////////////////////////////////////////////////
static const QColor __picStarsColor(Qt::green);
static const QColor __catStarsColor(Qt::magenta);
static const QColor __eqPicStarsColor(Qt::yellow);
static const QColor __eqCatStarsColor(Qt::cyan);
static const QColor __targetsColor(Qt::red);
static const int __picStarsPen   = 1;
static const int __catStarsPen   = 1;
static const int __eqPicStarsPen = 2;
static const int __eqCatStarsPen = 1;
static const int __targetsPen    = 2;
/////////////////////////////////////////////////////////////////////////////////////
class DetectorWindow : public ImageWindow
{
    Q_OBJECT
public:
    explicit DetectorWindow(const QString &title,
                            QSettings *s)
        : ImageWindow(title, s) {}
public slots:
    void setMode(int); //Detector::MODE
private:
    ArtifactVector  _picStars;
    ArtifactVector  _catStars;
    ArtifactVector  _eqPicStars;
    ArtifactVector  _eqCatStars;
    ArtifactVector  _targets;
    Detector::MODE  _mode;
    void drawAll();
    void clear();
private slots:
    void inputPicStars(ArtifactBox*);
    void inputCatStars(ArtifactBox*);
    void inputEquatedStars(ArtifactBox *pic,
                           ArtifactBox *cat);
    void inputTargets(ArtifactBox*);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // DETECTOR_WND_H
