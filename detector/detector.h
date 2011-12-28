#ifndef DETECTOR_H
#define DETECTOR_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QSettings>
#include <QtAlgorithms>
#include <QDebug>
#include <QPoint>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/frame.h"
#include "boxes/artifact.h"
#include "utils/timeutils.h"
#include "detector/detection.h"
#include "detector/accumulator.h"
/////////////////////////////////////////////////////////////////////////////////////
class Detector : public QThread
{
    Q_OBJECT
public:
    explicit Detector(QSettings*,
                      Frame*,
                      ArtifactBox*);
    ~Detector();
private:
    Detector(const Detector&) {}
    Detector& operator =(const Detector&) {return *this;}
    static const int _timeout = 40;
    static const int _magnThresh = 2;
    QSettings      *_settings;
    Frame          *_frame;
    ArtifactBox    *_artifactBox;
    Artifact        _target;
    Frame           _rawFrame;
    Accumulator     _accum;
    void cookArtifacts();
private slots:
    void inputFrame(Frame*,
                    int xTarget,
                    int yTarget);
signals:
    void sendFrame(Frame*);
    void screenStarsReady(ArtifactBox*,
                          double xTarget,
                          double yTarget);
};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // DETECTOR_H
