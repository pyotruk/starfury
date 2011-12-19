#ifndef STARDETECTOR_H
#define STARDETECTOR_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QSettings>
#include <QtAlgorithms>
#include <QDebug>
#include <QPoint>
/////////////////////////////////////////////////////////////////////////////////////
#include "opencv.hpp"
#include "utils/cvhelpfun.h"
#include "boxes/frame.h"
#include "math/astrocalc.h"
#include "utils/timeutils.h"
#include "boxes/artifact.h"
/////////////////////////////////////////////////////////////////////////////////////
class StarDetector : public QThread
{
    Q_OBJECT
public:
    explicit StarDetector(QSettings*,
                          ArtifactBox*);
    ~StarDetector();
private:
    StarDetector(const StarDetector&) {}
    StarDetector& operator =(const StarDetector&) {return *this;}
    static const int _timeout = 40;
    static const int _magnThresh = 2;
    QSettings      *_settings;
    ArtifactBox    *_artifactBox;
    Frame           _frame;
    Artifact        _target;
    void filtering(Frame&);
    void findArtifacts(Frame&,
                       ArtifactVector&,
                       const double thresh);
    void deleteTarget(ArtifactVector&,
                      const QPointF &target);
private slots:
    void inputFrame(Frame*,
                    int xTarget,
                    int yTarget);
signals:
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
#endif // STARDETECTOR_H
