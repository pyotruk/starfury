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
    _tribox.lock().lockForWrite();
    a->lock().lockForRead();

    _tribox.data().picStars = a->data();

    a->lock().unlock();

    QTime t;
    t.start();
    this->proc();
    qDebug() << "Angmeter proc delay: " << t.elapsed();


    _tribox.lock().unlock();

    emit sendTriangles(&_tribox);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputCatStars(ArtifactBox *a)
{
    _tribox.lock().lockForWrite();
    a->lock().lockForRead();

    _tribox.data().catStars = a->data();

    a->lock().unlock();
    _tribox.lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::proc()
{
    art::selectOnCircle(_tribox.data().picStars,
                        QPoint(_screen.width() / 2, _screen.height() / 2),
                        _screen.height() / 2);
    art::cutoff(_tribox.data().picStars,
                _maxStarQuantity);

    art::selectOnCircle(_tribox.data().catStars,
                        QPoint(_screen.width() / 2, _screen.height() / 2),
                        _screen.height() / 2);
    art::cutoff(_tribox.data().catStars,
                _maxStarQuantity);

    if(_tribox.data().catStars.size() > _tribox.data().picStars.size())
        art::cutoff(_tribox.data().catStars,
                    _tribox.data().picStars.size());

    tri::cookTriangles(_tribox.data().picStars,
                       _tribox.data().picTriangles);
    tri::deleteEqual(_tribox.data().picTriangles,
                     _equalEps);

    tri::cookTriangles(_tribox.data().catStars,
                       _tribox.data().catTriangles);
    tri::deleteEqual(_tribox.data().catTriangles,
                     _equalEps);

    tri::cookTriangleBox(_tribox.data(), _similarEps);
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
