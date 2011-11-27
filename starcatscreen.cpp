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
    _starcatReader->refresh(t->alpha, t->delta);

    _starBox->lock().lockForWrite();

    _starcatReader->mutex()->lock();
    this->proc(*t);
    _starcatReader->mutex()->unlock();

    winfiletime2qdatetime(t->timeUTC, _starBox->timeMarker());
    _starBox->lock().unlock();

    emit catStarsReady(_starBox);
    lock->unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::proc(TelescopeVector &t)
{
    _segment->generateNew(t.alpha,
                          t.delta,
                          t.fieldWidth,
                          t.fieldHeight);
    _starBox->artifacts().clear();
    Artifact star;
    StarVector::iterator it = _starcatReader->stars()->begin();
    for(; it != _starcatReader->stars()->end(); ++it)
    {
        if(_segment->isBelong(it->alpha(), it->delta()))
        {
            catStar2screenStar(t, *it, star);
            _starBox->artifacts().push_back(star);
        }
    }
    qSort(_starBox->artifacts());
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::catStar2screenStar(TelescopeVector &t,
                                       Star            &catStar,
                                       Artifact        &screenStar)
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
    screenStar.center() = QPoint(x, y);
    screenStar.setMagnitude(ac::calcStarRadius(qAbs(catStar.magnitude())));
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
