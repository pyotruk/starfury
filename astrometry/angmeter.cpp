#include "angmeter.h"
/////////////////////////////////////////////////////////////////////////////////////
Angmeter::Angmeter(QSettings *s,
                   ArtifactBox *equatedPicStars,
                   ArtifactBox *equatedCatStars,
                   TargetBox   *target) :
    _settings(s),
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
    _equatedStarQuantity = s->value(__skeyEquatedStarQuantity, _defaultEquatedStarQuantity).toInt();
    _equalEps = s->value(__skeyEqualEps, _defaultEqualEps).toDouble();
    _similarEps = s->value(__skeySimilarEps, _defaultSimilarEps).toDouble();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::saveSettings(QSettings *s)
{
    s->setValue(__skeyScreenWidth, _screen.width());
    s->setValue(__skeyScreenHeight, _screen.height());
    s->setValue(__skeyMaxStarQuantity, _maxStarQuantity);
    s->setValue(__skeyEquatedStarQuantity, _equatedStarQuantity);
    s->setValue(__skeyEqualEps, _equalEps);
    s->setValue(__skeySimilarEps, _similarEps);
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
void Angmeter::equation()
{
    QPointF screenCenter(_screen.width() / 2, _screen.height() / 2);
    art::selectOnCircle(_rawPicStars.data(),
                        screenCenter,
                        screenCenter.y());
    art::selectOnCircle(_rawCatStars.data(),
                        screenCenter,
                        screenCenter.y());
    art::cutoff(_rawPicStars.data(), _maxStarQuantity);
    art::cutoff(_rawCatStars.data(), _maxStarQuantity);
    id::equate(_rawPicStars.data(),
               _rawCatStars.data(),
               _similarEps,
               screenCenter);
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
    if(picStars.size() < _equatedStarQuantity)    return false;
    if(catStars.size() < _equatedStarQuantity)    return false;
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
            qDebug() << "!!! check equation fail: " << "\n"
                     << "       cat: " << *itCat << "\n"
                     << "       pic: " << *itPic << "\n"
                     << "       pic CONVERTED: " << "x = " << x << "    y = " << y;
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

    this->equation();

    LinCor cor;
    lincor::cook(_rawPicStars.data(),
                 _rawCatStars.data(),
                 cor);

    if(this->checkEquation(_rawPicStars.data(),
                           _rawCatStars.data(),
                           cor,
                           _equalEps))
    {
        qDebug() << "a1 = " << cor.a1
                 << "    b1 = " << cor.b1
                 << "    c1 = " << cor.c1;
        qDebug() << "a2 = " << cor.a2
                 << "    b2 = " << cor.b2
                 << "    c2 = " << cor.c2;

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
