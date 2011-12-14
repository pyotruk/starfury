#include "starcatscreen.h"
/////////////////////////////////////////////////////////////////////////////////////
StarcatScreen::StarcatScreen(QSettings *s,
                             ArtifactBox *a) :
    _settings(s),
    _starBox(a),
    _starcatReader(new StarcatReader(_settings)),
    _segment(new SkySegment(_starcatReader->segment().field().width(),
                            _starcatReader->segment().field().height(),
                            1, 0)),
    _snServer(new SnUdpSrv(_settings))
{
    this->moveToThread(this);
    qDebug() << "starcatScreen::_starcatReader thread: " << _starcatReader->thread();
    qDebug() << "starcatScreen::_snServer thread: " << _snServer->thread();
    this->loadSettings(_settings);
    QObject::connect(_snServer, SIGNAL(telescopeVectorReady(TelescopeVector*,QReadWriteLock*)),
                     this, SLOT(inputTelescopeVector(TelescopeVector*,QReadWriteLock*)),
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
    int screenWidth = s->value(SKEY_SCREEN_WIDTH, _defaultScreenWidth).toInt();
    int screenHeight = s->value(SKEY_SCREEN_HEIGHT, _defaultScreenHeight).toInt();
    this->setScreenSize(screenWidth, screenHeight);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::saveSettings(QSettings *s)
{
    s->setValue(SKEY_SCREEN_WIDTH, _screen.width());
    s->setValue(SKEY_SCREEN_HEIGHT, _screen.height());
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::inputTelescopeVector(TelescopeVector *t,
                                         QReadWriteLock  *lock)
{
    lock->lockForRead();
    _telescope = *t;
    lock->unlock();

    _starcatReader->refresh(_telescope.alpha,
                            _telescope.delta/*,
                            _telescope.fieldWidth,
                            _telescope.fieldHeight*/);

    _starBox->lock().lockForWrite();

    _starcatReader->mutex()->lock();
    this->proc(_telescope,
               *(_starcatReader->stars()),
               _starBox->data(),
               *_segment);
    _starcatReader->mutex()->unlock();

    winfiletime2qdatetime(_telescope.timeUTC, _starBox->timeMarker());
    _starBox->lock().unlock();

    emit catStarsReady(_starBox);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::inputTarget(int x, int y)
{
    double alpha, delta;
    this->screenTarget2catTarget(_telescope,
                                 x, y,
                                 alpha, delta);
    double errAlpha = (_telescope.alpha - alpha) * __rad2deg * 3600;
    double errDelta = (_telescope.delta - delta) * __rad2deg * 3600;
    qDebug() << "errAlpha = " << errAlpha
             << "    errDelta = " << errDelta;
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::proc(const TelescopeVector &t,
                         StarVector &s,
                         ArtifactVector &a,
                         SkySegment &seg)
{
    if(s.empty())  return;
    seg.generateNew(t.alpha,
                    t.delta,
                    _starcatReader->segment().field().width(),
                    _starcatReader->segment().field().height()
                    /*t.fieldWidth,
                    t.fieldHeight*/);
    a.clear();
    Artifact star;
    StarVector::iterator it = s.begin();
    for(; it < s.end(); ++it)
    {
        if(seg.isBelong(it->alpha(), it->delta()))
        {
            catStar2screenStar(t, *it, star);
            a.push_back(star);
        }
    }
    qSort(a.begin(), a.end(), qGreater<Artifact>());
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::catStar2screenStar(const TelescopeVector &t,
                                       Star &catStar,
                                       Artifact &screenStar)
{
    double starAzimuth, starElevation;
    double starAngleX, starAngleY;
    ac::iieqt2horiz(catStar.alpha(), catStar.delta(),
                    t.LST, t.latitude,
                    starAzimuth, starElevation);
    ac::horiz2screen(t.azHoriz, t.elHoriz,
                     starAzimuth, starElevation,
                     starAngleX, starAngleY);
    int x, y;
    ac::screenAngles2screenPoint(starAngleX,
                                 starAngleY,
                                 catStar.delta(),
                                 _segment->field(),
                                 _screen,
                                 x,
                                 y);
    screenStar.setCenter(QPoint(x, y));
    screenStar.setMagnitude(ac::calcStarRadius(qAbs(catStar.magnitude())));
}
////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::screenTarget2catTarget(const TelescopeVector &t,
                                           const int x,
                                           const int y,
                                           double &alpha,
                                           double &delta)
{
    double xAng, yAng;
    double azHoriz, elHoriz;
    ac::screenPoint2screenAngles(x,
                                 y,
                                 _telescope.delta,
                                 _starcatReader->segment().field(),
                                 _screen,
                                 xAng,
                                 yAng);
    ac::screen2horiz(_telescope.azHoriz,
                     _telescope.elHoriz,
                     xAng,
                     yAng,
                     azHoriz,
                     elHoriz);
    ac::horiz2iieqt(azHoriz,
                    elHoriz,
                    _telescope.LST,
                    _telescope.latitude,
                    alpha,
                    delta);
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
