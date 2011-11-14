#include "starcatscreen.h"
/////////////////////////////////////////////////////////////////////////////////////
StarcatScreen::StarcatScreen(QSettings *settings) :
    _settings(settings),
    _tvector(new TelescopeVector),
    _starcatReader(new StarcatReader(_settings)),
    _stars(new ArtifactVector),
    _mutex(new QMutex),
    _segment(new SkySegment(_starcatReader->segment().field().width(),
                            _starcatReader->segment().field().height(),
                            1, 0)),
    _snServer(new SnUdpSrv(_settings))
{
    this->moveToThread(this);
    qDebug() << "starcatScreen::_starcatReader thread: " << _starcatReader->thread();
    qDebug() << "starcatScreen::_snServer thread: " << _snServer->thread();
    this->loadSettings(_settings);
    QObject::connect(_snServer, SIGNAL(dataReady(TelescopeVector*,QMutex*)),
                     this, SLOT(telescopeVectorIn(TelescopeVector*,QMutex*)),
                     Qt::QueuedConnection);

    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
StarcatScreen::~StarcatScreen()
{
    this->quit();
    this->terminate();
    this->saveSettings(_settings);
    delete _snServer;
    delete _segment;
    delete _mutex;
    delete _stars;
    delete _starcatReader;
    delete _tvector;
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::loadSettings(QSettings *settings)
{
    int screenWidth = settings->value(SKEY_SCREEN_WIDTH, _defaultScreenWidth).toInt();
    int screenHeight = settings->value(SKEY_SCREEN_HEIGHT, _defaultScreenHeight).toInt();
    this->setScreenSize(screenWidth, screenHeight);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::saveSettings(QSettings *settings)
{
    settings->setValue(SKEY_SCREEN_WIDTH, _screen.width());
    settings->setValue(SKEY_SCREEN_HEIGHT, _screen.height());
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::telescopeVectorIn(TelescopeVector *tvector,
                                      QMutex *mutex)
{
    QObject::disconnect(_snServer, SIGNAL(dataReady(TelescopeVector*,QMutex*)),
                     this, SLOT(telescopeVectorIn(TelescopeVector*,QMutex*)));

    if(mutex->tryLock(_timeout))
    {
        *_tvector = *tvector;
        mutex->unlock();
        _starcatReader->refresh(_tvector->alpha, _tvector->delta);
        if(this->_mutex->tryLock(_timeout))
        {
            if(_starcatReader->mutex()->tryLock(_timeout))
            {
                this->processing();
                _starcatReader->mutex()->unlock();
            }
            winfiletime2qdatetime(_tvector->timeUTC, _time);
            this->_mutex->unlock();
            emit starsReady(_stars, this->_mutex, &_time);
        }
    }

    QObject::connect(_snServer, SIGNAL(dataReady(TelescopeVector*,QMutex*)),
                     this, SLOT(telescopeVectorIn(TelescopeVector*,QMutex*)),
                     Qt::QueuedConnection);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::processing()
{
    _segment->generateNew(_tvector->alpha,
                          _tvector->delta,
                          _tvector->fieldWidth,
                          _tvector->fieldHeight);
    _stars->clear();
    Artifact star;
    StarVector::iterator it = _starcatReader->stars()->begin();
    for(; it != _starcatReader->stars()->end(); ++it)
    {
        if(_segment->isBelong(it->alpha(), it->delta()))
        {
            catStar2screenStar(*it, star);
            _stars->push_back(star);
        }
    }
    qSort(*_stars);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::catStar2screenStar(Star     &catStar,
                                       Artifact &screenStar)
{
    double starAzimuth, starElevation;
    double starAngleX, starAngleY;
    ac::iieqt2horiz(catStar.alpha(), catStar.delta(),
                    _tvector->LST, _tvector->latitude,
                    starAzimuth, starElevation);
    ac::horiz2screen(_tvector->azHoriz, _tvector->elHoriz,
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
