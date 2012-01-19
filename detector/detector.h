#ifndef DETECTOR_H
#define DETECTOR_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <QThread>
#include <QSettings>
#include <QtAlgorithms>
#include <QPointF>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/frame.h"
#include "boxes/artifact.h"
#include "utils/timeutils.h"
#include "detector/detector_hf.h"
#include "detector/accumulator.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
static const QString __skeyAccumCapacity("/Detector/AccumCapacity");
static const QString __skeyDetectorMode("/Detector/Mode");
/////////////////////////////////////////////////////////////////////////////////////
class Detector : public QThread
{
    Q_OBJECT
public:
    enum MODE {TARGET_DETECTION = 0, STAR_DETECTION = 1};

    explicit Detector(QSettings*,
                      FrameBox*,
                      ArtifactBox *stars,
                      ArtifactBox *targets);
    ~Detector();
    const Accumulator& accum() const {return _accum;}
    MODE mode() const {return _mode;}
public slots:
    void setAccumCapacity(int cap) {_accum.setCapacity(cap);}
    void setBinEnabled(bool b)     {_binEnabled = b;}
    void setMode(Detector::MODE m) {_mode = m;  _accum.clear();}
    void setMode(int m) {this->setMode((MODE)m);}
private:
    Detector(const Detector&) {}
    Detector& operator =(const Detector&) {return *this;}
    static const int _timeout = 40;
    static const MODE _defaultMode = STAR_DETECTION;
    static const int _smoothingKernelSize = 7;
    QSettings      *_settings;
    FrameBox       *_frame;
    ArtifactBox    *_stars;
    ArtifactBox    *_targets;
    FrameBox        _cache_Frame;
    ArtifactBox     _cache_Stars;
    ArtifactBox     _cache_Targets;
    Accumulator     _accum;
    bool            _binEnabled;
    MODE            _mode;
    QPointF         _velocity;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void accumulate(const MODE);
    void detectStars();
    void detectTargets();
    void accumIsFull();
private slots:
    void inputFrame(FrameBox*);
    void inputScreenVelocity(const double vx,
                             const double vy);
signals:
    void sendFrame(FrameBox*);
    void starsReady(ArtifactBox*);
    void targetsReady(ArtifactBox*);
};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // DETECTOR_H
