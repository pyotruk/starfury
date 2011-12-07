#include "angmeter.h"
/////////////////////////////////////////////////////////////////////////////////////
Angmeter::Angmeter(QSettings *s) :
    _settings(s)
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
    int screenWidth = s->value(SKEY_SCREEN_WIDTH, _defaultScreenWidth).toInt();
    int screenHeight = s->value(SKEY_SCREEN_HEIGHT, _defaultScreenHeight).toInt();
    this->setScreenSize(screenWidth, screenHeight);
    _maxStarQuantity = s->value(SKEY_MAX_STAR_QUANTITY, _defaultMaxStarQuantity).toInt();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::saveSettings(QSettings *s)
{
    s->setValue(SKEY_SCREEN_WIDTH, _screen.width());
    s->setValue(SKEY_SCREEN_HEIGHT, _screen.height());
    s->setValue(SKEY_MAX_STAR_QUANTITY, _maxStarQuantity);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputScreenStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _screenStars = *a;
    a->lock().unlock();

    this->proc(_screenStars.artifacts(),
               _catStars.artifacts());
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputCatStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _catStars = *a;
    a->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::proc(ArtifactVector &screenStars,
                    ArtifactVector &catStars)
{
    this->selection(screenStars,
                    _screen.height() / 2,
                    _maxStarQuantity);
    this->selection(catStars,
                    _screen.height() / 2,
                    _maxStarQuantity);

    TriangleVector screenTriangles;
    TriangleVector catTriangles;
    triangle::cookTriangles(screenStars, screenTriangles);
    triangle::cookTriangles(catStars, catTriangles);

    _equatedScreenStars.lock().lockForWrite();
    _equatedCatStars.lock().lockForWrite();
    triangle::cookEquatedArtifacts(screenTriangles,
                                   catTriangles,
                                   _equatedScreenStars.artifacts(),
                                   _equatedCatStars.artifacts());
    _equatedCatStars.lock().unlock();
    _equatedScreenStars.lock().unlock();

    emit sendEquatedScreenStars(&_equatedScreenStars);
    emit sendEquatedCatStars(&_equatedCatStars);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::setScreenSize(const int width,
                             const int height)
{
    _screen.setWidth(width);
    _screen.setHeight(height);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::selection(ArtifactVector &a,
                        const int radius,
                        const int maxQuantity)
{
    double r = (double)radius;
    double dist;
    QPoint cen(_screen.width() / 2,
               _screen.height() / 2);
    ArtifactVector::iterator it = a.begin();
    for(; it < a.end(); ++it)
    {
        dist = ac::calcDistance(cen, it->center());
        if(dist > r)
        {
            a.erase(it);
        }
    }
    int dif = a.size() - maxQuantity;
    if(dif > 0)     a.erase(a.end() - dif, a.end());
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
