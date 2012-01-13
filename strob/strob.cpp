#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
Strob::Strob(QSettings *s) :
    _settings(s),
    _geometry(new StrobGeometry(_settings))
{
    loadSettings(_settings);
}
/////////////////////////////////////////////////////////////////////////////////////
Strob::~Strob()
{
    saveSettings(_settings);
    delete _geometry;
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::loadSettings(QSettings *s)
{
    _threshold = s->value(__skeyStrobStddevThreshold, _defaultThreshold).toDouble();
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::saveSettings(QSettings *s)
{
    s->setValue(__skeyStrobStddevThreshold, _threshold);
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::makeTracking(Frame &f)
{
    //��������, ������������� ����������� � �������� �������
    if(!_geometry->checkRange(QSize(f.header().width(),
                                    f.header().height())))
    {
        qDebug() << "Strob says: geometry->checkRange() FAILED";
        return;
    }
    cv::Rect signalRect;
    cv::Rect foneRect;
    cvhelp::qtRect2cvRect(_geometry->signalRect(), signalRect);
    cvhelp::qtRect2cvRect(_geometry->foneRect(), foneRect);
    cv::Mat signalRoi(f.asCvMat(), signalRect);
    cv::Mat foneRoi(f.asCvMat(), foneRect);
    cv::blur(foneRoi, foneRoi, cv::Size(5,5)); //����������
    cv::blur(signalRoi, signalRoi, cv::Size(5,5)); //����������

    double sumThreshold; /*����� �� ��������� ��������� �������
                         � ���������� � ������� �������*/
    calcThresholds(signalRoi,
                   foneRoi,
                   _threshold, //����� � �������� ���
                   sumThreshold,
                   _pixThreshold,
                   _signalMean,
                   _foneMean);

    if(sumThreshold > 0)    //�������� ������� ��������
    {
        cv::threshold(signalRoi, //��������� ����������� � ���������� ������
                      signalRoi,
                      _pixThreshold,
                      0xFF,
                      cv::THRESH_TOZERO);

        //���������� ������ ���� �� ����������� ������
        cv::TermCriteria crit(cv::TermCriteria::COUNT, 1, 0.1);
        cv::meanShift(f.asCvMat(), signalRect, crit);
        QRect newSignalRect;
        cvhelp::cvRect2qtRect(signalRect, newSignalRect);
        _geometry->setRect(newSignalRect);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::calcThresholds(const cv::Mat &signalRoi,
                           const cv::Mat &foneRoi,
                           const double stdDevThreshold,
                           double &sumThreshold,
                           int    &pixThreshold,
                           double &signalMean,
                           double &foneMean)
{
    double sumSignal = cv::sum(signalRoi)[0]; //����� ������� �������� �� ����������� ������
    signalMean = sumSignal / signalRoi.total();
    double sumFone = cv::sum(foneRoi)[0]; //����� ������� �������� �� �������� ������
    foneMean = sumFone / foneRoi.total();
    double sumStdDev = qSqrt(sumFone); //CKO
    double stdDevPerPix = sumStdDev / foneRoi.total();
    sumThreshold = sumSignal - sumFone; /*����� �� ��������� ��������� �������
                                          � ���������� � ������� �������*/
    pixThreshold = (int)qFloor(0.5 + foneMean + stdDevThreshold * stdDevPerPix); //�����, ���������� � ������ �������
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
