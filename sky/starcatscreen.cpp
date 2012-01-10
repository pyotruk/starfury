#include "starcatscreen.h"
/////////////////////////////////////////////////////////////////////////////////////
StarcatScreen::StarcatScreen(QSettings *s,
                             LogFile *log,
                             ArtifactBox *a) :
    _settings(s),
    _log(log),
    _starBox(a),
    _starcatReader(new StarcatReader(_settings)),
    _segment(new SkySegment(_starcatReader->segment().field().width(),
                            _starcatReader->segment().field().height(),
                            1, 0)),
    _snServer(new SnUdpSrv(_settings))
{
    this->moveToThread(this);
    qDebug() << "starcatScreen._starcatReader " << _starcatReader->thread();
    qDebug() << "starcatScreen._snServer " << _snServer->thread();
    this->loadSettings(_settings);
    QObject::connect(_snServer, SIGNAL(telescopeStatusReady(TelescopeBox*)),
                     this, SLOT(inputTelescopeStatus(TelescopeBox*)),
                     Qt::QueuedConnection);

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
void StarcatScreen::inputTelescopeStatus(TelescopeBox *t)
{
    if(_telescopeVec.size() > _maxTelescopeVectorSize)
        _telescopeVec.clear();

    t->lock().lockForRead();
    _telescopeVec.push_back(t->data());
    TelescopeStatus tscope = t->data();
    _velocimeter.addPoint(QPointF(_screen.width() / 2,
                                  _screen.height() / 2),
                          *t,
                          _starcatReader->segment().field(),
                          _screen);
    t->lock().unlock();

    qDebug() << "screen velocity: " << _velocimeter.vel().x()
             << "  " << _velocimeter.vel().y();
    emit sendScreenVelocity(_velocimeter.vel().x(),
                            _velocimeter.vel().y());

    _starcatReader->refresh(tscope.alpha,
                            tscope.delta);

    _starBox->lock().lockForWrite();

    _starcatReader->mutex()->lock();
    this->cookStars(tscope,
                    *(_starcatReader->stars()),
                    _starBox->data(),
                    *_segment);
    _starcatReader->mutex()->unlock();

    QDateTime tMark = _starBox->timeMarker();
    timeutils::winfiletime2qdatetime(tscope.timeUTC, tMark);
    _starBox->setTimeMarker(tMark);

    _starBox->lock().unlock();

    emit catStarsReady(_starBox);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::inputTarget(ArtifactBox *target)
{
    target->lock().lockForRead();
    if(target->data().empty())
    {
        target->lock().unlock();
        return;
    }
    Artifact picTarget = target->data().front();
    QDateTime t = target->timeMarker();
    target->lock().unlock();

    qint64 minDelay;
    TelescopeStatus tscope = telescope::findNearestByTime(t,
                                                          _telescopeVec,
                                                          minDelay);
    qDebug() << "min delay: " << minDelay;

    Star catTarget;
    this->screenStar2catStar(tscope,
                             picTarget,
                             catTarget);
    double errAlpha = tscope.alpha - catTarget.alpha();
    double errDelta = tscope.delta - catTarget.delta();
    emit sendMeasureError(errAlpha, errDelta);
    double errAlphaMin = errAlpha * __rad2deg * 3600;
    double errDeltaMin = errDelta * __rad2deg * 3600;
    _log->write(QString::number(errAlphaMin) + " " +
                QString::number(errDeltaMin) + " " +
                QString::number(catTarget.alpha()) + " " +
                QString::number(catTarget.delta()) + " " +
                QString::number(tscope.alpha) + " " +
                QString::number(tscope.delta) + " " +
                QString::number(minDelay) + " ");
    qDebug() << "errAlpha = " << errAlphaMin
             << "    errDelta = " << errDeltaMin;
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::cookStars(const TelescopeStatus &t,
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
void StarcatScreen::catStar2screenStar(const TelescopeStatus &t,
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
void StarcatScreen::screenStar2catStar(const TelescopeStatus &t,
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
