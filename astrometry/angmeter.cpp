#include "angmeter.h"
/////////////////////////////////////////////////////////////////////////////////////
Angmeter::Angmeter(QSettings *s,
                   LogFile *log,
                   ArtifactBox *equatedPicStars,
                   ArtifactBox *equatedCatStars,
                   TargetBox   *target) :
    _settings(s),
    _log(log),
    _equatedPicStars(equatedPicStars),
    _equatedCatStars(equatedCatStars),
    _target(target)
{
    this->moveToThread(this);
    qDebug() << "equator thread: " << this->thread();
    this->loadSettings(_settings);
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
Angmeter::~Angmeter()
{
    this->quit();
    this->wait();
    this->saveSettings(_settings);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::loadSettings(QSettings *s)
{
    int screenWidth = s->value(__skeyScreenWidth, _defaultScreenWidth).toInt();
    int screenHeight = s->value(__skeyScreenHeight, _defaultScreenHeight).toInt();
    this->setScreenSize(screenWidth, screenHeight);
    _maxStarQuantity = s->value(__skeyMaxStarQuantity, _defaultMaxStarQuantity).toInt();
    _minEquatedStarQuantity = s->value(__skeyMinEquatedStarQuantity, _defaultMinEquatedStarQuantity).toInt();
    _equalEps = s->value(__skeyEqualEps, _defaultEqualEps).toDouble();
    _similarEps = s->value(__skeySimilarEps, _defaultSimilarEps).toDouble();
    _nearStarDist = s->value(__skeyNearStarDist, _defaultNearStarDist).toDouble();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::saveSettings(QSettings *s)
{
    s->setValue(__skeyScreenWidth, _screen.width());
    s->setValue(__skeyScreenHeight, _screen.height());
    s->setValue(__skeyMaxStarQuantity, _maxStarQuantity);
    s->setValue(__skeyMinEquatedStarQuantity, _minEquatedStarQuantity);
    s->setValue(__skeyEqualEps, _equalEps);
    s->setValue(__skeySimilarEps, _similarEps);
    s->setValue(__skeyNearStarDist, _nearStarDist);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::setScreenSize(const int width,
                             const int height)
{
    _screen.setWidth(width);
    _screen.setHeight(height);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputScreenStars(ArtifactBox *a,
                                double xTarget,
                                double yTarget)
{
    a->lock().lockForRead();
    _rawPicStars = *a;
    a->lock().unlock();

    QTime t;
    t.start();

    this->proc(QPointF(xTarget, yTarget));

    qDebug() << "Angmeter proc delay: " << t.elapsed();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputCatStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _rawCatStars = *a;
    a->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::correctTarget(const LinCor &cor,
                             Artifact &target)
{
    double xCat, yCat;
    lincor::conversion(cor,
                       target.center().x(),
                       target.center().y(),
                       xCat, yCat);
    double dx = xCat - target.center().x();
    double dy = yCat - target.center().y();
    qDebug() << "dx = " << dx
             << "    dy = " << dy;
    target.setCenter(QPoint(xCat, yCat));
}
/////////////////////////////////////////////////////////////////////////////////////
bool Angmeter::checkEquation(const ArtifactVector &picStars,
                             const ArtifactVector &catStars,
                             const LinCor &cor,
                             const double equalEps)
{
    double x, y;
    for(ArtifactVector::const_iterator itPic = picStars.constBegin(),
                                       itCat = catStars.constBegin();
        itPic < picStars.constEnd();
        ++itPic, ++itCat)
    {
        lincor::conversion(cor,
                           itPic->center().x(),
                           itPic->center().y(),
                           x,
                           y);
        if(!art::isEqual(*itCat, Artifact(x, y), equalEps))
        {
            qDebug() << "!!! check equation FAIL: " << "\n";
            return false;
        }
    }
    qDebug() << "check equation SUCCESS";
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////
bool Angmeter::checkTimeDelay(const ArtifactBox &a1,
                              const ArtifactBox &a2,
                              const int maxDelay)
{
    int dt = qAbs(a1.timeMarker().time().msecsTo(a2.timeMarker().time()));
    if(dt > maxDelay)
    {
        return false;
    }
    else
    {
        return true;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::cookTarget(const QPointF &target,
                          const QDateTime &timeMarker,
                          const LinCor    &cor,
                          TargetBox &targetBox)
{
    targetBox.data().setCenter(target);
    this->correctTarget(cor, targetBox.data());
    targetBox.setTimeMarker(timeMarker);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::proc(const QPointF &target)
{
    if(!this->checkTimeDelay(_rawPicStars, _rawCatStars, _maxDelay))
    {
        return;
    }

    int ret = identifier::equate(_rawPicStars.data(),
                                 _rawCatStars.data(),
                                 _screen,
                                 _similarEps,
                                 _nearStarDist,
                                 _maxStarQuantity,
                                 _minEquatedStarQuantity);
    if(ret != identifier::__SUCCESS)
    {
        _log->write(QString::number(ret));
        return;
    }

    _log->write(QString::number(_rawPicStars.data().front().center().x()) + " " + //pic refStar0
                QString::number(_rawPicStars.data().front().center().y()) + " " +
                QString::number(_rawPicStars.data().front().magnitude())  + " " +

                QString::number(_rawPicStars.data()[1].center().x()) + " " + //pic refStar1
                QString::number(_rawPicStars.data()[1].center().y()) + " " +
                QString::number(_rawPicStars.data()[1].magnitude())  + " " +

                QString::number(_rawCatStars.data().front().center().x()) + " " + //cat refStar0
                QString::number(_rawCatStars.data().front().center().y()) + " " +
                QString::number(_rawCatStars.data().front().magnitude())  + " " +

                QString::number(_rawCatStars.data()[1].center().x()) + " " + //cat refStar1
                QString::number(_rawCatStars.data()[1].center().y()) + " " +
                QString::number(_rawCatStars.data()[1].magnitude())  + " ");

    LinCor cor;
    lincor::cook(_rawPicStars.data(),
                 _rawCatStars.data(),
                 cor);

    if(this->checkEquation(_rawPicStars.data(),
                           _rawCatStars.data(),
                           cor,
                           _equalEps))
    {
        _equatedPicStars->lock().lockForWrite();
        _equatedCatStars->lock().lockForWrite();
        *_equatedPicStars = _rawPicStars;
        *_equatedCatStars = _rawCatStars;
        _equatedCatStars->lock().unlock();
        _equatedPicStars->lock().unlock();
        emit sendEquatedStars(_equatedPicStars, _equatedCatStars);

        _target->lock().lockForWrite();
        this->cookTarget(target,
                         _rawPicStars.timeMarker(),
                         cor,
                         *_target);
        _target->lock().unlock();
        emit sendTarget(_target);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
