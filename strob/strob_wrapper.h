#ifndef STROB_WRAPPER_H
#define STROB_WRAPPER_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QThread>
#include <QMouseEvent>
/////////////////////////////////////////////////////////////////////////////////////
#include "strob/strob.h"
/////////////////////////////////////////////////////////////////////////////////////
class StrobWrapper : public QThread
{
    Q_OBJECT
public:
    explicit StrobWrapper(QSettings*,
                          LogFile*,
                          ArtifactBox *targets);
    ~StrobWrapper();
    inline const Strob& strob() const {return *_strob;}
private:
    static const int _timeout = 20;
    StrobWrapper(const StrobWrapper&) {}
    StrobWrapper& operator =(const StrobWrapper&) {return *this;}
    LogFile     *_log;
    ArtifactBox *_targets;
    Strob       *_strob;
private slots:
    void inputFrame(FrameBox*);
    void targetsDetected();
    void setPos(QMouseEvent*);
    void setSide(const int);
    void setVelocity(const double vx,
                     const double vy);
signals:
    void frameReady(FrameBox*);
    void freshTargets(ArtifactBox*);
    void sendPhotometry(double timeMarker, //sec.000
                        double signal,
                        double fone);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROB_WRAPPER_H
