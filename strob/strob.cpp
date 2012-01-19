#include "strob.h"
/////////////////////////////////////////////////////////////////////////////////////
Strob::Strob(QSettings *s) :
    _settings(s),
    _locked(false)
{
    loadSettings(_settings);
    _geometry.moveToRefPoint();
}
/////////////////////////////////////////////////////////////////////////////////////
Strob::~Strob()
{
    this->killTimer(_timerId);
    saveSettings(_settings);
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::loadSettings(QSettings *s)
{
    _threshold = s->value(__skeyStrobStddevThreshold, _defaultThreshold).toDouble();
    this->setRefPoint(QPoint(s->value(__skeyStrobRefPointX, Geometry::_defaultRefPointX).toInt(),
                             s->value(__skeyStrobRefPointY, Geometry::_defaultRefPointY).toInt()));
    this->setSide(s->value(__skeyStrobSide, Geometry::_defaultSide).toInt());
}
///////////////////////////////////////////////////////////////////////////////////////
void Strob::saveSettings(QSettings *s)
{
    s->setValue(__skeyStrobStddevThreshold, _threshold);
    s->setValue(__skeyStrobSide, _geometry.signalRect().width());
    s->setValue(__skeyStrobRefPointX, _geometry.refPoint().x());
    s->setValue(__skeyStrobRefPointY, _geometry.refPoint().y());
}
///////////////////////////////////////////////////////////////////////////////////////
Strob::RETURN_VALUES Strob::proc(Frame &f)
{
    _geometry.setFrameSize(QSize(f.header().width(),
                                 f.header().height()));
    if(!_geometry.isValid())    return BAD_GEOMETRY;

    //создание, инициализация сигнального и фонового стробов
    cv::Mat signalRoi(f.asCvMat(), _geometry.signalCvRect());
    cv::Mat foneRoi(f.asCvMat(), _geometry.foneCvRect());
    cv::blur(foneRoi, foneRoi, cv::Size(5,5)); //фильтрация
    cv::blur(signalRoi, signalRoi, cv::Size(5,5)); //фильтрация

    double sumThreshold; /* порог по суммарным значениям яркости
                            в сигнальном и фоновом стробах */
    strob_hf::calcThresholdsAndRatings(signalRoi,
                                       foneRoi,
                                       _threshold, //порог в единицах СКО
                                       sumThreshold,
                                       _pixThreshold,
                                       _signalMean,
                                       _foneMean);

    if(_locked)    return LOCKED;

    if(sumThreshold > 0)    //проверка условия слежения
    {
        cv::threshold(signalRoi, //пороговая бинаризация в сигнальном стробе
                      signalRoi,
                      _pixThreshold,
                      0xFF,
                      cv::THRESH_TOZERO);
        QPoint newCenter;
        strob_hf::calcCenterOfMass(f.asCvMat(),
                                   _geometry.signalRect(),
                                   newCenter);
        _geometry.setCenter(newCenter);
    }
    else
    {
        int lockTime = strob_hf::calcLockTime(_velocity,
                                              _geometry.side());
        if(lockTime > 0)
        {
            _locked = true;
            _timerId = this->startTimer(lockTime);
            qDebug() << "Strob says: i`m LOCKED, lock time = " << lockTime;
        }
    }
    return OK;
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::timerEvent(QTimerEvent *)
{
    this->unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::unlock()
{
    this->killTimer(_timerId);
    _locked = false;
    qDebug() << "Strob says: " << "i`m UNLOCKED !";
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::setSide(const int s)
{
    _geometry.setSide(s);
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::setCenter(const QPoint &p)
{
    _geometry.setCenter(p);
    if(_locked)    this->unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::setCenter(const QPointF &p)
{
    this->setCenter(QPoint((int)p.x(),
                           (int)p.y()));
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::setRefPoint(const QPoint &p)
{
    _geometry.setRefPoint(p);
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::setVelocity(const QPointF &v)
{
    _geometry.setVelocity(v);
    _velocity = QVector2D(v);
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::toArtifact(Artifact &a)
{
    a.setCenter(QPointF((double)_geometry.center().x(),
                        (double)_geometry.center().y()));
    a.setMagnitude((double)_geometry.side());
}
/////////////////////////////////////////////////////////////////////////////////////
void Strob::moveToRefPoint()
{
    this->setCenter(_geometry.refPoint());
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

