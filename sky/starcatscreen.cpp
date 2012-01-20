#include "starcatscreen.h"
/////////////////////////////////////////////////////////////////////////////////////
StarcatScreen::StarcatScreen(QSettings   *s,
                             ArtifactBox *shared_CatStars) :
    _settings(s),
    _shared_CatStars(shared_CatStars),
    _catReader(new StarcatReader(s)),
    _segment(new SkySegment(_catReader->segment().field().width(),
                            _catReader->segment().field().height(),
                            1, 0))
{
    this->moveToThread(this);
    _velocimeter.moveToThread(this);
    this->loadSettings(_settings);

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
    delete _segment;
    delete _catReader;
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
void StarcatScreen::inputTelPos(TelBox *t)
{
    t->lock().lockForRead();
    _cache_TelPos = *t;
    t->lock().unlock();

    _velocimeter.addPoint(QPointF(_screen.width() / 2,
                                  _screen.height() / 2),
                                  _cache_TelPos,
                                  _catReader->segment().field(),
                                  _screen);

    _catReader->refresh(_cache_TelPos.data().alpha,
                        _cache_TelPos.data().delta);

    _catReader->mutex()->lock();
    this->cookStars(_cache_TelPos,
                    *(_catReader->stars()),
                    _cache_CatStars);
    _catReader->mutex()->unlock();

    _shared_CatStars->lock().lockForWrite();
    *_shared_CatStars = _cache_CatStars;
    _shared_CatStars->lock().unlock();

    emit catStarsReady(_shared_CatStars);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatScreen::cookStars(const TelBox     &t,
                              const StarVector &s,
                              ArtifactBox      &a)
{
    if(s.empty())  return;
    _segment->generateNew(t.data().alpha,
                          t.data().delta,
                          _catReader->segment().field().width(),
                          _catReader->segment().field().height());
    a.data().clear();
    for(StarVector::const_iterator
        it = s.constBegin(); it < s.constEnd(); ++it)
    {
        if(_segment->isBelong(it->alpha(), it->delta()))
        {
            Artifact star;
            astrometry::catStar2screenStar(*it,
                                           star,
                                           t.data(),
                                           _segment->field(),
                                           _screen);
            a.data().push_back(star);
        }
    }
    qSort(a.data().begin(), a.data().end());
    a.setTimeMarker(t.timeMarker());
}
/////////////////////////////////////////////////////////////////////////////////////
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
