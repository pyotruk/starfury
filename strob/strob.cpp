#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
Strob::Strob(QSettings *s,
             LogFile *log) :
    _settings(s),
    _log(log),
    _geometry(new StrobGeometry(_settings))
{
    this->moveToThread(this);
    _geometry->moveToThread(this);
    loadSettings(_settings);
    qDebug() << "strob->_geometry thread:" << _geometry->thread();
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
Strob::~Strob()
{
    this->quit();
    this->wait();
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
void Strob::makeTracking(Frame *f)
{
    //��������, ������������� ����������� � �������� �������
    _geometry->checkRange(QSize(f->header().width,
                                f->header().height));
    cv::Rect innerRect;
    cv::Rect outerRect;
    cvhelp::qtRect2cvRect(_geometry->innerRect(), innerRect);
    cvhelp::qtRect2cvRect(_geometry->outerRect(), outerRect);
    cv::Mat signalRoi(f->asCvMat(), innerRect);
    cv::Mat foneRoi(f->asCvMat(), outerRect);

    cv::blur(foneRoi, foneRoi, cv::Size(5,5)); //����������

    double sumThreshold; /*����� �� ��������� ��������� �������
                         � ���������� � ������� �������*/
    calcThresholds(signalRoi,
                   foneRoi,
                   _threshold, //����� � �������� ���
                   sumThreshold,
                   _pixThreshold);

    if(sumThreshold > 0)    //�������� ������� ��������
    {
        cv::threshold(signalRoi, //��������� ����������� � ���������� ������
                      signalRoi,
                      _pixThreshold,
                      0xFF,
                      cv::THRESH_TOZERO);

        //���������� ������ ���� �� ����������� ������
        cv::TermCriteria crit(cv::TermCriteria::COUNT, 1, 0.1);
        cv::meanShift(f->asCvMat(), innerRect, crit);
        QRect newInnerRect;
        cvhelp::cvRect2qtRect(innerRect, newInnerRect);
        _geometry->setRect(newInnerRect);
    }

    int dx, dy;
    _geometry->getError(dx, dy);
    _log->write(QString::number(dx) + " " +
                QString::number(dy));
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::calcThresholds(const cv::Mat &signalRoi,
                           const cv::Mat &foneRoi,
                           const double stdDevThreshold,
                           double &sumThreshold,
                           int    &pixThreshold)
{
    double sumSignal = cv::sum(signalRoi)[0]; //����� ������� �������� �� ����������� ������
    double sumFone = cv::sum(foneRoi)[0]; //�� �������� ������
    sumFone -= sumSignal; //�� �����
    double sumStdDev = sqrt(sumFone); //CKO
    sumThreshold = sumSignal - sumFone; /*����� �� ��������� ��������� �������
                                          � ���������� � ������� �������*/
    int borderPixNum = (foneRoi.total() - signalRoi.total()); //���-�� �������� �� �����
    double fonePerPix = sumFone / borderPixNum;
    double stdDevPerPix = sumStdDev / borderPixNum;
    pixThreshold = (int)floor(0.5 + fonePerPix + stdDevThreshold * stdDevPerPix); //�����, ���������� � ������ �������
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::clickTarget(QMouseEvent *mousePressEvent)
{
    _geometry->setCenter(mousePressEvent->pos());
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::setThreshold(const int pos)
{
    _threshold = (double)pos;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////