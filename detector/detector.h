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
//setting keys
static const QString __skeyAccumCapacity("/Detector/AccumCapacity");
/////////////////////////////////////////////////////////////////////////////////////
class Detector : public QThread
{
    Q_OBJECT
public:
    explicit Detector(QSettings*,
                      Frame*,
                      ArtifactBox*);
    ~Detector();
    const Accumulator& accum() const {return _accum;}
public slots:
    void setAccumCapacity(int cap) {_accum.setCapacity(cap);}
    void setBinEnabled(bool b)     {_binEnabled = b;}
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
    bool            _binEnabled;
    void cookArtifacts();
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
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
