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
    _equalEps = s->value(SKEY_EQUAL_EPS, _defaultEqualEps).toDouble();
    _similarEps = s->value(SKEY_SIMILAR_EPS, _defaultSimilarEps).toDouble();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::saveSettings(QSettings *s)
{
    s->setValue(SKEY_SCREEN_WIDTH, _screen.width());
    s->setValue(SKEY_SCREEN_HEIGHT, _screen.height());
    s->setValue(SKEY_MAX_STAR_QUANTITY, _maxStarQuantity);
    s->setValue(SKEY_EQUAL_EPS, _equalEps);
    s->setValue(SKEY_SIMILAR_EPS, _similarEps);
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
    art::selectOnCircle(screenStars,
                        QPoint(_screen.width() / 2, _screen.height() / 2),
                        _screen.height() / 2);
    art::cutoff(screenStars, _maxStarQuantity);

    art::selectOnCircle(catStars,
                        QPoint(_screen.width() / 2, _screen.height() / 2),
                        _screen.height() / 2);
    art::cutoff(catStars, _maxStarQuantity);

    if(catStars.size() > screenStars.size())
        art::cutoff(catStars, screenStars.size());

    TriangleVector screenTriangles;
    TriangleVector catTriangles;

    tri::cookTriangles(screenStars, screenTriangles);
    tri::deleteEqual(screenTriangles, _equalEps);

    tri::cookTriangles(catStars, catTriangles);
    tri::deleteEqual(catTriangles, _equalEps);

    _equatedScreenStars.lock().lockForWrite();
    _equatedCatStars.lock().lockForWrite();
    tri::cookEquatedArtifacts(screenTriangles,
                              catTriangles,
                              _similarEps,
                              _equatedScreenStars.artifacts(),
                              _equatedCatStars.artifacts());
    art::deleteEqual(_equatedScreenStars.artifacts(), _equalEps);
    art::deleteEqual(_equatedCatStars.artifacts(), _equalEps);
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
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
