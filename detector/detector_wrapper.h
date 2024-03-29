/* [DetectorWrapper]:
   ����������� ����� ������ ��� ������ [Detector].
   ��������� � ������ [Detector] �� ���� ����������
   (����� ���������� ��������� ������������ �����, ����� ��������)
   �������� ����� ������� (~140ms i3/2.5GHz) � [Detector] �� ������������
   � FPS=25Hz � ���������� �����. �� ����� ��������� [Detector]
   �� ����� ������������ ������� � ��� ������������� � �������,
   ��� �������� � overflow, memory leak.
   [DetectorWrapper] ������������ ������� � ��������� [Detector]
   ����� <QMutex>.
*/
#ifndef DETECTOR_WRAPPER_H
#define DETECTOR_WRAPPER_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
/////////////////////////////////////////////////////////////////////////////////////
#include "detector/target_detector.h"
#include "detector/star_detector.h"
/////////////////////////////////////////////////////////////////////////////////////
/* setting keys */
static const QString __skeyDetectorMode("/Detector/Mode");
/////////////////////////////////////////////////////////////////////////////////////
class DetectorWrapper : public QThread
{
    Q_OBJECT
public:
    enum MODE {TARGET_DETECTION = 0, STAR_DETECTION = 1};

    explicit DetectorWrapper(QSettings*,
                             FrameBox *shared_Frame,
                             ArtifactBox *shared_Stars,
                             ArtifactBox *shared_Targets);
    ~DetectorWrapper();

    inline int mode() const {return (int)_mode;}
    inline int accumCapacity() const {return _cache_AccumCap;}

public slots:
    inline void setAccumCapacity(int cap) {_cache_AccumCap = cap;}
    void setMode(int m);

private:
    DetectorWrapper(const DetectorWrapper&) {}
    DetectorWrapper& operator =(const DetectorWrapper&) {return *this;}

    static const int  _timeout = 20;
    static const MODE _defaultMode = STAR_DETECTION;

    /* shared */
    QSettings      *_settings;
    FrameBox       *_shared_Frame;
    ArtifactBox    *_shared_Stars;
    ArtifactBox    *_shared_Targets;

    TargetDetector *_targetDetector;
    StarDetector   *_starDetector;

    MODE _mode;

    int       _cache_AccumCap;
    QVector2D _cache_Vel;

    void do_Target_Detection(FrameBox*);
    void do_Star_Detection(FrameBox*);

    void loadSettings(QSettings*);
    void saveSettings(QSettings*);

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
#endif // DETECTOR_WRAPPER_H
