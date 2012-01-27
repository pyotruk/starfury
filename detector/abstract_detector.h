#ifndef ABSTRACT_DETECTOR_H
#define ABSTRACT_DETECTOR_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <QThread>
#include <QSettings>
#include <QtAlgorithms>
#include <QMutex>
/////////////////////////////////////////////////////////////////////////////////////
#include "boxes/frame.h"
#include "boxes/artifact.h"
#include "utils/timeutils.h"
#include "detector/detector_hf.h"
#include "detector/accumulator.h"
/////////////////////////////////////////////////////////////////////////////////////
/* setting keys */
static const QString __skeyAccumCapacity("/Detector/AccumCapacity");
static const QString __skeyMinSquare("/Detector/MinSquare");
static const QString __skeyOblongDiff("/Detector/OblongDiff");
/////////////////////////////////////////////////////////////////////////////////////
class AbstractDetector : public QThread
{
public:
    explicit AbstractDetector(QSettings*);
    ~AbstractDetector();
    const FrameBox&    frame(bool binarize = false);
    const ArtifactBox& artifacts();
    inline QMutex& mutex()    {return _mutex;}
    inline bool ready() const {return _ready;}
    inline void setAccumCapacity(int cap) {_accum.setCapacity(cap);}
    inline void clearAccum()              {_accum.clear();}
    void inputFrame(FrameBox*);
protected:
    static const int _blurKernelSize = 7;
    int          _minSquare;   /* нижний порог по площади найденного объекта */
    int          _maxOblong;   /* верхний порог по коэффициенту вытянутости объекта */
    FrameBox     _frame;
    ArtifactBox  _artifacts;   /* найденные объекты */
    Accumulator  _accum;
    virtual void run();
    virtual void accumulate() = 0;
private:
    AbstractDetector(const AbstractDetector&) {}
    AbstractDetector& operator =(const AbstractDetector&) {return *this;}
    static const int _defaultMinSquare = 9;
    static const int _defaultOblongDiff = 2;
    QSettings   *_settings; //shared
    QMutex       _mutex;
    bool         _ready;    /* флаг готовности: обнаружены объекты */
    void detect();
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // ABSTRACT_DETECTOR_H
