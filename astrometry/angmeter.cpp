#include "angmeter.h"
/////////////////////////////////////////////////////////////////////////////////////
Angmeter::Angmeter(QSettings *s,
                   LogFile *starsLog,
                   LogFile *targetLog,
                   ArtifactBox *shared_eqPicStars,
                   ArtifactBox *shared_eqCatStars) :
    _settings(s),
    _starsLog(starsLog),
    _targetLog(targetLog),
    _shared_EqPicStars(shared_eqPicStars),
    _shared_EqCatStars(shared_eqCatStars)
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
    int screenWidth = s->value(__skeyScreenWidth, __defaultScreenWidth).toInt();
    int screenHeight = s->value(__skeyScreenHeight, __defaultScreenHeight).toInt();
    this->setScreenSize(screenWidth, screenHeight);

    double fieldWidth = s->value(__skeyFieldWidth, __defaultFieldWidth).toDouble() * ac::_dmin2rad;
    double fieldHeight = s->value(__skeyFieldHeight, __defaultFieldHeight).toDouble() * ac::_dmin2rad;
    this->setFieldSize(fieldWidth, fieldHeight);

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
    s->setValue(__skeyFieldWidth, _field.width() * ac::_rad2dmin);
    s->setValue(__skeyFieldHeight, _field.height() * ac::_rad2dmin);
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
void Angmeter::setFieldSize(const double width,
                            const double height)
{
    _field.setWidth(width);
    _field.setHeight(height);
}
/////////////////////////////////////////////////////////////////////////////////////
bool Angmeter::cacheIsEmpty()
{
    if(_cache_PicStars.data().empty())    return true;
    if(_cache_CatStars.data().empty())    return true;
    if(_cache_Targets.data().empty())     return true;
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
bool Angmeter::queueIsEmpty()
{
    if(_queue_CatStars.empty())    return true;
    if(_queue_Targets.empty())     return true;
    if(_queue_TelPos.empty())      return true;
    return false;
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::refreshCache()
{
    _cache_CatStars = _queue_CatStars.head();
    _queue_CatStars.clear();

    _cache_Targets = _queue_Targets.head();
    _queue_Targets.clear();

    _cache_TelPos = _queue_TelPos.head();
    _queue_TelPos.clear();
}
/////////////////////////////////////////////////////////////////////////////////////
bool Angmeter::checkQueueDelay()
{
    int delay = timeutils::msecBetween(_cache_PicStars.timeMarker(),
                                       _cache_CatStars.timeMarker());
    qDebug() << "Angmeter: queue delay = " << delay;
    if(delay > _maxDelay)    return false;
    else    return true;
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputPicStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _cache_PicStars = *a;
    a->lock().unlock();

    if(this->queueIsEmpty())
    {
        qDebug() << "Angmeter: queue is empty";
        return;
    }

    this->refreshCache();

    if(this->cacheIsEmpty())
    {
        qDebug() << "Angmeter: cache is empty";
        return;
    }

    if(!this->checkQueueDelay())
    {
        qDebug() << "Angmeter: too big queue delay";
        return;
    }

    QTime t;    t.start();
    if(this->procStars())    this->procTargets();
    qDebug() << "Angmeter: proc delay = " << t.elapsed();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputCatStars(ArtifactBox *a)
{
    if(_queue_CatStars.size() > _maxQueueSize)
    {
        _queue_CatStars.clear();
        qDebug() << "Angmeter: [_queue_CatStars] OVERFLOW, was cleared";
    }
    a->lock().lockForRead();
    _queue_CatStars.enqueue(*a);
    a->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputTargets(ArtifactBox *t)
{
    if(_queue_Targets.size() > _maxQueueSize)
    {
        _queue_Targets.clear();
        qDebug() << "Angmeter: [_queue_Targets] OVERFLOW, was cleared";
    }
    t->lock().lockForRead();
    _queue_Targets.enqueue(*t);
    t->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputTelPos(TelBox *t)
{
    if(_queue_TelPos.size() > _maxQueueSize)
    {
        _queue_TelPos.clear();
        qDebug() << "Angmeter: [_queue_TelPos] OVERFLOW, was cleared";
    }
    t->lock().lockForRead();
    _queue_TelPos.enqueue(*t);
    t->lock().unlock();
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
            return false;
        }
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////
bool Angmeter::procStars()
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
    if(ret != astrometry::SUCCESS)    return false;

    _starsLog->write(QString::number(_cache_PicStars.data().front().center().x()) + " " + //pic refStar0
                     QString::number(_cache_PicStars.data().front().center().y()) + " " +

                     QString::number(_cache_PicStars.data()[1].center().x()) + " " + //pic refStar1
                     QString::number(_cache_PicStars.data()[1].center().y()) + " " +

                     QString::number(_cache_CatStars.data().front().center().x()) + " " + //cat refStar0
                     QString::number(_cache_CatStars.data().front().center().y()) + " " +

                     QString::number(_cache_CatStars.data()[1].center().x()) + " " + //cat refStar1
                     QString::number(_cache_CatStars.data()[1].center().y()) + " ");

    lincor::cook(_cache_PicStars.data(),
                 _cache_CatStars.data(),
                 _lincor);

    if(this->checkEquation(_cache_PicStars.data(),
                           _cache_CatStars.data(),
                           _lincor,
                           _equalEps))
    {
        _shared_EqPicStars->lock().lockForWrite();
        _shared_EqCatStars->lock().lockForWrite();
        *_shared_EqPicStars = _cache_PicStars;
        *_shared_EqCatStars = _cache_CatStars;
        _shared_EqCatStars->lock().unlock();
        _shared_EqPicStars->lock().unlock();
        emit sendEquatedStars(_shared_EqPicStars,
                              _shared_EqCatStars);
        qDebug() << "Angmeter: procStars(): check equation SUCCESS";
        return true;
    }
    else
    {
        qDebug() << "Angmeter: procStars(): check equation FAIL";
        return false;
    }

}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::procTargets()
{
    Artifact picTarget = _cache_Targets.data().front();
    this->correctTarget(_lincor, picTarget);
    Star catTarget;
    astrometry::screenStar2catStar(picTarget,
                                   catTarget,
                                   _cache_TelPos.data(),
                                   _field,
                                   _screen);
    double errAlpha /*rad*/ = _cache_TelPos.data().alpha - catTarget.alpha();
    double errDelta /*rad*/ = _cache_TelPos.data().delta - catTarget.delta();

    emit sendMeasureError(errAlpha, errDelta);

    double errAlphaSec = errAlpha * ac::_rad2dsec;
    double errDeltaSec = errDelta * ac::_rad2dsec;
    _targetLog->write(QString::number(errAlphaSec) + " " +
                      QString::number(errDeltaSec) + " " +
                      QString::number(catTarget.alpha()) + " " +
                      QString::number(catTarget.delta()) + " " +
                      QString::number(_cache_TelPos.data().alpha) + " " +
                      QString::number(_cache_TelPos.data().delta) + " ");
    qDebug() << "Angmeter: errAlpha = " << errAlphaSec << "  errDelta = " << errDeltaSec;
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
    target.setCenter(QPointF(xCat, yCat));
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
