#include "angmeter.h"
/////////////////////////////////////////////////////////////////////////////////////
Angmeter::Angmeter(QSettings *s,
                   LogFile *log,
                   ArtifactBox *picStars,
                   ArtifactBox *catStars,
                   ArtifactBox *targets) :
    _settings(s),
    _log(log),
    _picStars(picStars),
    _catStars(catStars),
    _targets(targets)
{
    this->moveToThread(this);
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
void Angmeter::inputTargets(ArtifactBox *targets)
{
    if(_queue_Targets.size() > _maxQueueSize)
    {
        _queue_Targets.clear();
        qDebug() << "Angmeter: _queue_Targets OVERFLOW, was cleared";
    }
    targets->lock().lockForRead();
    _queue_Targets.enqueue(*targets);
    targets->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputCatStars(ArtifactBox *a)
{
    if(_queue_CatStars.size() > _maxQueueSize)
    {
        _queue_CatStars.clear();
        qDebug() << "Angmeter: _queue_CatStars OVERFLOW, was cleared";
    }
    a->lock().lockForRead();
    _queue_CatStars.enqueue(*a);
    a->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputPicStars(ArtifactBox *stars)
{
    stars->lock().lockForRead();
    _cache_PicStars = *stars;
    stars->lock().unlock();

    if(_cache_PicStars.data().empty())
    {
        qDebug() << "Angmeter: [_cache_PicStars] is EMPTY !";
        _queue_CatStars.clear();
        _queue_Targets.clear();
        return;
    }
    if(_queue_CatStars.empty())
    {
        qDebug() << "Angmeter: [_queue_CatStars] is EMPTY !";
        _queue_CatStars.clear();
        _queue_Targets.clear();
        return;
    }
    if(_queue_Targets.empty())
    {
        qDebug() << "Angmeter: [_queue_Targets] is EMPTY !";
        _queue_CatStars.clear();
        _queue_Targets.clear();
        return;
    }

    if(!this->refreshQueueAndCheckDelay())    return;

    QTime t;    t.start();
    this->proc();
    qDebug() << "Angmeter: proc delay = " << t.elapsed();
}
/////////////////////////////////////////////////////////////////////////////////////
bool Angmeter::refreshQueueAndCheckDelay()
{
    _cache_CatStars = _queue_CatStars.head();
    _queue_CatStars.clear();
    int delay = timeutils::msecBetween(_cache_PicStars.timeMarker(),
                                       _cache_CatStars.timeMarker());
    qDebug() << "Angmeter: [PicStars-CatStars] delay = " << delay;
    if(delay > _maxDelay)
    {
        qDebug() << "Angmeter: too big time delay between [PicStars-CatStars]";
        return false;
    }

    _cache_Targets = _queue_Targets.head();
    _queue_Targets.clear();
    delay = timeutils::msecBetween(_cache_PicStars.timeMarker(),
                                   _cache_Targets.timeMarker());
    qDebug() << "Angmeter: [PicStars-Targets] delay = " << delay;
    if(delay > _maxDelay)
    {
        qDebug() << "Angmeter: too big time delay between [PicStars-Targets]";
        return false;
    }

    return true;
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
    qDebug() << "Angmeter: correctTarget(),  dx = " << dx << "  dy = " << dy;
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
            qDebug() << "Angmeter: check equation FAIL";
            return false;
        }
    }
    qDebug() << "Angmeter: check equation SUCCESS";
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::cookTarget(const LinCor &cor)
{
    Artifact target = _cache_Targets.data().front();
    _cache_Targets.data().clear();
    this->correctTarget(cor, target);
    _cache_Targets.data().push_back(target);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::proc()
{
    art::deleteTargets(_cache_PicStars.data(), _cache_Targets.data());

    int ret = astrometry::equate(_cache_PicStars.data(),
                                 _cache_CatStars.data(),
                                 _screen,
                                 _similarEps,
                                 _nearStarDist,
                                 _maxStarQuantity,
                                 _minEquatedStarQuantity,
                                 _method);
    if(ret != astrometry::SUCCESS)    return;

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
        _picStars->lock().lockForWrite();
        _catStars->lock().lockForWrite();
        *_picStars = _cache_PicStars;
        *_catStars = _cache_CatStars;
        _catStars->lock().unlock();
        _picStars->lock().unlock();
        emit sendEquatedStars(_picStars, _catStars);

        this->cookTarget(cor);
        _targets->lock().lockForWrite();
        *_targets = _cache_Targets;
        _targets->lock().unlock();
        emit sendTargets(_targets);
    }
    else    emit sendTargets(0);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
