#include "starcatscreen.h"
/////////////////////////////////////////////////////////////////////////////////////
StarcatScreen::StarcatScreen(QSettings *settings) :
    _settings(settings),
    _tvector(new TelescopeVector),
    _starcatReader(new StarcatReader(_settings)),
    _stars(new ArtifactVector),
    _mutex(new QMutex),
    _field(_starcatReader->segment().field())
{
    this->moveToThread(this);
    qDebug() << "starcatScreen::_starcatReader thread: " << _starcatReader->thread();
    this->loadSettings(_settings);
    this->start(QThread::NormalPriority);
}
/////////////////////////////////////////////////////////////////////////////////////
StarcatScreen::~StarcatScreen()
{
    this->quit();
    this->terminate();
    this->saveSettings(_settings);
    delete _mutex;
    delete _stars;
    delete _starcatReader;
    delete _tvector;
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::loadSettings(QSettings *settings)
{
    double screenWidth = settings->value(SKEY_SCREEN_WIDTH, _defaultScreenWidth).toInt();
    double screenHeight = settings->value(SKEY_SCREEN_HEIGHT, _defaultScreenHeight).toInt();
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
    if(mutex->tryLock(_timeout))
    {
        *_tvector = *tvector;
        mutex->unlock();
        _starcatReader->refresh(_tvector->alpha, _tvector->delta);
        if(_mutex->tryLock(_timeout))
        {
            if(_starcatReader->mutex()->tryLock(_timeout))
            {
                this->processing();
                _starcatReader->mutex()->unlock();
            }
            _mutex->unlock();
            emit starsReady(_stars, _mutex);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::processing()
{
    Artifact star;
    StarVector::iterator it = _starcatReader->stars()->begin();
    for(; it != _starcatReader->stars()->end(); ++it)
    {
        if(!_segment.isBelong(it->alpha(), it->delta()))
        {
            catStar2screenStar(*it, star);
            _stars->push_back(star);
        }
    }
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
                     starAngleX, starAngleY );
    int x, y;
    ac::screenAngles2screenPoint(starAngleX,
                                 starAngleY,
                                 catStar.delta(),
                                 _field,
                                 _screen,
                                 x,
                                 y);
    screenStar.center() = QPoint(x, y);
    screenStar.setMagnitude(catStar.magnitude());
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::setScreenSize(const int width,
                                  const int height)
{
    _field.setWidth(width);
    _field.setHeight(height);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
