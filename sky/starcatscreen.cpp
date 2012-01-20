#include "starcatscreen.h"
/////////////////////////////////////////////////////////////////////////////////////
StarcatScreen::StarcatScreen(QSettings *s,
                             LogFile *log,
                             ArtifactBox *a) :
    _settings(s),
    _log(log),
    _starBox(a),
    _starcatReader(new StarcatReader(s)),
    _segment(new SkySegment(_starcatReader->segment().field().width(),
                            _starcatReader->segment().field().height(),
                            1, 0)),
    _snServer(new SnUdpSrv(s))
{
    this->moveToThread(this);
    _velocimeter.moveToThread(this);
    qDebug() << "StarcatScreen: StarcatReader " << _starcatReader->thread();
    qDebug() << "StarcatScreen: SnServer " << _snServer->thread();
    this->loadSettings(_settings);

    QObject::connect(_snServer, SIGNAL(sendTelescopePos(TelescopeBox*)),
                     this, SLOT(inputTelescopePos(TelescopeBox*)),
                     Qt::QueuedConnection);
    QObject::connect(&_velocimeter, SIGNAL(velocityReady(double,double)),
                     this, SIGNAL(sendScreenVelocity(double,double)),
                     Qt::DirectConnection);

    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
StarcatScreen::~StarcatScreen()
{
    this->quit();
    this->wait();
    this->saveSettings(_settings);
    delete _snServer;
    delete _segment;
    delete _starcatReader;
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::loadSettings(QSettings *s)
{
    int screenWidth = s->value(__skeyScreenWidth, _defaultScreenWidth).toInt();
    int screenHeight = s->value(__skeyScreenHeight, _defaultScreenHeight).toInt();
    this->setScreenSize(screenWidth, screenHeight);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::saveSettings(QSettings *s)
{
    s->setValue(__skeyScreenWidth, _screen.width());
    s->setValue(__skeyScreenHeight, _screen.height());
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::inputTelescopePos(TelescopeBox *t)
{
    if(_queue_TelescopePos.size() > _maxQueueSize)
    {
        _queue_TelescopePos.clear();
        qDebug() << "StarcatScreen: _queue_TelescopePos OVERFLOW, was cleared";
    }
    t->lock().lockForRead();
    _queue_TelescopePos.enqueue(*t);
    TelescopeBox telescopePos(*t);
    t->lock().unlock();

    _velocimeter.addPoint(QPointF(_screen.width() / 2,
                                  _screen.height() / 2),
                          telescopePos,
                          _starcatReader->segment().field(),
                          _screen);

    _starcatReader->refresh(telescopePos.data().alpha,
                            telescopePos.data().delta);

    _starBox->lock().lockForWrite();
    _starcatReader->mutex()->lock();
    this->cookStars(telescopePos.data(),
                    *(_starcatReader->stars()),
                    _starBox->data(),
                    *_segment);
    _starcatReader->mutex()->unlock();
    _starBox->setTimeMarker(telescopePos.timeMarker());
    _starBox->lock().unlock();

    emit catStarsReady(_starBox);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::inputTargets(ArtifactBox *targets)
{
    if(targets == 0) //это значит, что отождествление не прошло проверку
    {
        _queue_TelescopePos.clear();
        qDebug() << "StarcatScreen: check equation FAIL";
        return;
    }

    targets->lock().lockForRead();
    _cache_Targets = *targets;
    targets->lock().unlock();

    if(_cache_Targets.data().empty())
    {
        qDebug() << "StarcatScreen: [picTarget] is EMPTY !";
        return;
    }
    if(_queue_TelescopePos.empty())
    {
        qDebug() << "StarcatScreen: [_queue_TelescopePos] is EMPTY !";
        return;
    }

    if(!this->refreshQueueAndCheckDelay())    return;

    Star catTarget;
    this->screenStar2catStar(_cache_TelescopePos.data(),
                             _cache_Targets.data().front(),
                             catTarget);
    double errAlpha = _cache_TelescopePos.data().alpha - catTarget.alpha();
    double errDelta = _cache_TelescopePos.data().delta - catTarget.delta();
    emit sendMeasureError(errAlpha, errDelta);
    double errAlphaMin = errAlpha * ac::_rad2deg * 3600;
    double errDeltaMin = errDelta * ac::_rad2deg * 3600;
    _log->write(QString::number(errAlphaMin) + " " +
                QString::number(errDeltaMin) + " " +
                QString::number(catTarget.alpha()) + " " +
                QString::number(catTarget.delta()) + " " +
                QString::number(_cache_TelescopePos.data().alpha) + " " +
                QString::number(_cache_TelescopePos.data().delta) + " ");
    qDebug() << "StarcatScreen: errAlpha = " << errAlphaMin << "  errDelta = " << errDeltaMin;
}
/////////////////////////////////////////////////////////////////////////////////////
bool StarcatScreen::refreshQueueAndCheckDelay()
{
    _cache_TelescopePos = _queue_TelescopePos.head();
    _queue_TelescopePos.clear();
    int delay = timeutils::msecBetween(_cache_TelescopePos.timeMarker(),
                                       _cache_Targets.timeMarker());
    qDebug() << "StarcatScreen: [TelescopePos-Targets] delay = " << delay;
    if(delay > _maxDelay)
    {
        qDebug() << "StarcatScreen: too big time delay between [TelescopePos-Targets]";
        return false;
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::cookStars(const TelescopePos &t,
                              const StarVector &s,
                              ArtifactVector &a,
                              SkySegment &seg)
{
    if(s.empty())  return;
    seg.generateNew(t.alpha,
                    t.delta,
                    _starcatReader->segment().field().width(),
                    _starcatReader->segment().field().height());
    a.clear();
    Artifact star;
    for(StarVector::const_iterator
        it = s.constBegin(); it < s.constEnd(); ++it)
    {
        if(seg.isBelong(it->alpha(), it->delta()))
        {
            catStar2screenStar(t, *it, star);
            a.push_back(star);
        }
    }
    qSort(a.begin(), a.end());
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::catStar2screenStar(const TelescopePos &t,
                                       const Star &s,
                                       Artifact &a)
{
    double starAzimuth, starElevation;
    double starAngleX, starAngleY;
    ac::iieqt2horiz(s.alpha(), s.delta(),
                    t.LST, t.latitude,
                    starAzimuth, starElevation);
    ac::horiz2screenAngles(t.azHoriz, t.elHoriz,
                           starAzimuth, starElevation,
                           starAngleX, starAngleY);
    double x, y;
    ac::screenAngles2screenPoint(starAngleX,
                                 starAngleY,
                                 s.delta(),
                                 _segment->field(),
                                 _screen,
                                 x,
                                 y);
    a.setCenter(QPointF(x, y));
    a.setMagnitude(s.magnitude());
}
////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::screenStar2catStar(const TelescopePos &t,
                                       const Artifact &a,
                                       Star &s)
{
    double xAng, yAng;
    double azHoriz, elHoriz;
    double alpha, delta;
    ac::screenPoint2screenAngles(a.center().x(),
                                 a.center().y(),
                                 t.delta,
                                 _starcatReader->segment().field(),
                                 _screen,
                                 xAng,
                                 yAng);
    ac::screenAngles2horiz(t.azHoriz,
                           t.elHoriz,
                           xAng,
                           yAng,
                           azHoriz,
                           elHoriz);
    ac::horiz2iieqt(azHoriz,
                    elHoriz,
                    t.LST,
                    t.latitude,
                    alpha,
                    delta);
    s.setAlpha(alpha);
    s.setDelta(delta);
}
////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::setScreenSize(const int width,
                                  const int height)
{
    _screen.setWidth(width);
    _screen.setHeight(height);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
