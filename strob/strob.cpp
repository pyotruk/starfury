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
    _geometry->checkRange(QSize(f.header().width(),
                                f.header().height()));
    cv::Rect innerRect;
    cv::Rect outerRect;
    cvhelp::qtRect2cvRect(_geometry->innerRect(), innerRect);
    cvhelp::qtRect2cvRect(_geometry->outerRect(), outerRect);
    cv::Mat signalRoi(f.asCvMat(), innerRect);
    cv::Mat foneRoi(f.asCvMat(), outerRect);
    cv::blur(foneRoi, foneRoi, cv::Size(5,5)); //����������

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
        cv::meanShift(f.asCvMat(), innerRect, crit);
        QRect newInnerRect;
        cvhelp::cvRect2qtRect(innerRect, newInnerRect);
        _geometry->setRect(newInnerRect);
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
    double sumFone = cv::sum(foneRoi)[0]; //�� �������� ������
    sumFone -= sumSignal; //�� �����
    double sumStdDev = sqrt(sumFone); //CKO
    sumThreshold = sumSignal - sumFone; /*����� �� ��������� ��������� �������
                                          � ���������� � ������� �������*/
    int borderPixNum = (foneRoi.total() - signalRoi.total()); //���-�� �������� �� �����
    foneMean = sumFone / borderPixNum;
    double stdDevPerPix = sumStdDev / borderPixNum;
    pixThreshold = (int)floor(0.5 + foneMean + stdDevThreshold * stdDevPerPix); //�����, ���������� � ������ �������
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
