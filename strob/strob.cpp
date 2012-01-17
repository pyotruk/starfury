#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
Strob::Strob(QSettings *s) :
    _settings(s),
    _geometry(new StrobGeometry(_settings)),
    _locked(false)
{
    loadSettings(_settings);
}
/////////////////////////////////////////////////////////////////////////////////////
Strob::~Strob()
{
    this->killTimer(_timerId);
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
    if(!_geometry->checkRange(QSize(f.header().width(),
                                    f.header().height())))
    {
        qDebug() << "Strob says: geometry->checkRange() FAILED";
        return;
    }

    //��������, ������������� ����������� � �������� �������
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

    if(_locked)
    {
        qDebug() << "Strob says: sorry, i`m LOCKED !";
        return;
    }

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
    else
    {
        _locked = true;
        int dt = this->lockTime();
        _timerId = this->startTimer(dt);
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
int Strob::lockTime() const
{
    QVector2D v = _geometry->velocity();
    if(v.isNull())
    {
        qDebug() << "Strob says: velocity is NULL";
        return 0;
    }
    int side = _geometry->side();
    double r = side * _sqrt2 * _lockSizeKoef;
    double dt = r / v.length() * 1000;
    qDebug() << "Strob says: _LOCK_" << "\n"
             << "   v = " << v << "\n"
             << "   side = " << side << "\n"
             << "   r = " << r << "\n"
             << "   dt = " << dt;
    return (int)dt;
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::timerEvent(QTimerEvent *event)
{
    if(_timerId == event->timerId())
    {
        this->unlock();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::unlock()
{
    this->killTimer(_timerId);
    _locked = false;
    qDebug() << "Strob says: _UNLOCK_";
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
