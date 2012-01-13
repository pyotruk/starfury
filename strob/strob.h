#ifndef STROB_H
#define STROB_H
/////////////////////////////////////////////////////////////////////////////////////
#include <numeric>
#include <QPoint>
#include <qmath.h>
#include <QSize>
#include <QSettings>
#include <QString>
/////////////////////////////////////////////////////////////////////////////////////
#include "strob/strobgeometry.h"
#include "utils/cvhelpfun.h"
#include "boxes/frame.h"
#include "common/logfile.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
static const QString __skeyStrobStddevThreshold("/Strob/StdDevThreshold");
/////////////////////////////////////////////////////////////////////////////////////
class Strob
{
public:
    explicit Strob(QSettings*);
    ~Strob();
    double threshold()        const {return _threshold;}
    int    pixThreshold()     const {return _pixThreshold;}
    StrobGeometry &geometry() const {return *_geometry;}
    void makeTracking(Frame&);
    void setThreshold(const double t) {_threshold = t;}
    double foneMean()   const {return _foneMean;}
    double signalMean() const {return _signalMean;}
private:
    Strob(const Strob&) {}
    Strob& operator =(const Strob&) {return *this;}
    static const double _defaultThreshold = 1.0;
    QSettings     *_settings;
    StrobGeometry *_geometry;
    double        _threshold;       //������������ (������) ����� ����������� � �������� ���
    int           _pixThreshold;    //����� �� ������� ������� (������������ ��� ����������� cv::threshold)
    double        _foneMean;        //������� � ������� ������ �� �������
    double        _signalMean;      //������� � ���������� ������ �� �������
    void calcThresholds(const cv::Mat &signalRoi,
                        const cv::Mat &foneRoi,
                        const double stdDevThreshold,
                        double &sumThreshold,
                        int    &pixThreshold,
                        double &signalMean,
                        double &foneMean);
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROB_H
