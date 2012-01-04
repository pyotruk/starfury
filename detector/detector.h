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
    enum MODE {TARGET_DETECTION = 0, STAR_DETECTION = 1};

    explicit Detector(QSettings*,
                      FrameBox*,
                      ArtifactBox *stars,
                      ArtifactBox *target);
    ~Detector();
    const Accumulator& accum() const {return _accum;}
    void setMode(MODE m) {_mode = m;}
public slots:
    void setAccumCapacity(int cap) {_accum.setCapacity(cap);}
    void setBinEnabled(bool b)     {_binEnabled = b;}
private:
    Detector(const Detector&) {}
    Detector& operator =(const Detector&) {return *this;}
    static const int _timeout = 40;
    static const int _magnThresh = 2;
    QSettings      *_settings;
    FrameBox       *_frame;
    ArtifactBox    *_stars;
    ArtifactBox    *_target;
    FrameBox        _cache_Frame;
    ArtifactBox     _cache_Stars;
    ArtifactBox     _cache_Target;
    Accumulator     _accum;
    bool            _binEnabled;
    MODE            _mode;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
    void cookArtifacts();
    void preproc();
    void detectStars();
    void detectTarget();
private slots:
    void inputFrame(FrameBox*);
    void inputTarget(int xTarget,
                     int yTarget);
signals:
    void sendFrame(FrameBox*);
    void screenStarsReady(ArtifactBox *stars,
                          ArtifactBox *target);
    void targetDetected(ArtifactBox*);
};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // DETECTOR_H
