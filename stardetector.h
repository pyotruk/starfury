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
#include "cvhelpfun.h"
#include "frame.h"
#include "artifactbox.h"
#include "astrocalc.h"
#include "adapters.h"
/////////////////////////////////////////////////////////////////////////////////////
class StarDetector : public QThread
{
    Q_OBJECT
public:
    explicit StarDetector(QSettings*,
                          ArtifactBox*);
    ~StarDetector();
private:
    static const int _timeout = 40;
    static const int _magnThresh = 2;
    QSettings      *_settings;
    ArtifactBox    *_artifactBox;
    Frame           _frame;
    QPoint          _target;
    void filtering(Frame&);
    void findArtifacts(Frame&,
                       ArtifactVector&,
                       double thresh);
    void deleteTarget(ArtifactVector&,
                      QPoint &target);
private slots:
    void inputFrame(Frame*,
                    int xTarget,
                    int yTarget);
signals:
    void screenStarsReady(ArtifactBox*);
};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STARDETECTOR_H