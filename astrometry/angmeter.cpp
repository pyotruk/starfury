#include "angmeter.h"
/////////////////////////////////////////////////////////////////////////////////////
Angmeter::Angmeter(QSettings *s,
                   LogFile *log,
                   ArtifactBox *equatedPicStars,
                   ArtifactBox *equatedCatStars,
                   ArtifactBox *target) :
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
    _method = (astrometry::METHOD)s->value(__skeyAstrometryMethod, _defaultMethod).toInt();
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
    s->setValue(__skeyAstrometryMethod, _method);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::setScreenSize(const int width,
                             const int height)
{
    _screen.setWidth(width);
    _screen.setHeight(height);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputScreenStars(ArtifactBox *stars,
                                ArtifactBox *target)
{
    stars->lock().lockForRead();
    _cache_PicStars = *stars;
    stars->lock().unlock();

    target->lock().lockForRead();
    _cache_Target = *target;
    target->lock().unlock();

    if(_cache_Target.data().empty())    return;

    QTime t;
    t.start();

    this->proc();

    qDebug() << "Angmeter proc delay: " << t.elapsed();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputCatStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _cache_CatStars = *a;
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
void Angmeter::cookTarget(const LinCor &cor)
{
    Artifact target = _cache_Target.data().front();
    _cache_Target.data().clear();
    this->correctTarget(cor, target);
    _cache_Target.data().push_back(target);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::proc()
{
    if(!timeutils::checkTimeDelay(_cache_PicStars.timeMarker(),
                                  _cache_CatStars.timeMarker(),
                                  _maxDelay))
    {
        return;
    }

    int ret = astrometry::equate(_cache_PicStars.data(),
                                 _cache_CatStars.data(),
                                 _screen,
                                 _similarEps,
                                 _nearStarDist,
                                 _maxStarQuantity,
                                 _minEquatedStarQuantity,
                                 _method);
    if(ret != astrometry::__SUCCESS)    return;

    _log->write(QString::number(_cache_PicStars.data().front().center().x()) + " " + //pic refStar0
                QString::number(_cache_PicStars.data().front().center().y()) + " " +
                QString::number(_cache_PicStars.data().front().magnitude())  + " " +

                QString::number(_cache_PicStars.data()[1].center().x()) + " " + //pic refStar1
                QString::number(_cache_PicStars.data()[1].center().y()) + " " +
                QString::number(_cache_PicStars.data()[1].magnitude())  + " " +

                QString::number(_cache_CatStars.data().front().center().x()) + " " + //cat refStar0
                QString::number(_cache_CatStars.data().front().center().y()) + " " +
                QString::number(_cache_CatStars.data().front().magnitude())  + " " +

                QString::number(_cache_CatStars.data()[1].center().x()) + " " + //cat refStar1
                QString::number(_cache_CatStars.data()[1].center().y()) + " " +
                QString::number(_cache_CatStars.data()[1].magnitude())  + " ");

    LinCor cor;
    lincor::cook(_cache_PicStars.data(),
                 _cache_CatStars.data(),
                 cor);

    if(this->checkEquation(_cache_PicStars.data(),
                           _cache_CatStars.data(),
                           cor,
                           _equalEps))
    {
        _equatedPicStars->lock().lockForWrite();
        _equatedCatStars->lock().lockForWrite();
        *_equatedPicStars = _cache_PicStars;
        *_equatedCatStars = _cache_CatStars;
        _equatedCatStars->lock().unlock();
        _equatedPicStars->lock().unlock();
        emit sendEquatedStars(_equatedPicStars, _equatedCatStars);

        this->cookTarget(cor);
        _target->lock().lockForWrite();
        *_target = _cache_Target;
        _target->lock().unlock();
        emit sendTarget(_target);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
