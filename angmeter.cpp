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
void Angmeter::inputScreenStars(ArtifactBox *a,
                                int xTarget,
                                int yTarget)
{
    a->lock().lockForRead();
    _picStars = a->data();
    a->lock().unlock();

    QTime t;
    t.start();

    _tribox.lock().lockForWrite();
    this->equation();
    _tribox.lock().unlock();

    qDebug() << "Angmeter equation delay: " << t.elapsed();

    emit sendTriangles(&_tribox);

    this->measureWork(xTarget, yTarget);
    emit sendTarget(xTarget, yTarget);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputCatStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _catStars = a->data();
    a->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::equation()
{
    art::selectOnCircle(_picStars,
                        QPoint(_screen.width() / 2, _screen.height() / 2),
                        _screen.height() / 2);
    art::cutoff(_picStars,
                _maxStarQuantity);

    art::selectOnCircle(_catStars,
                        QPoint(_screen.width() / 2, _screen.height() / 2),
                        _screen.height() / 2);
    art::cutoff(_catStars,
                _maxStarQuantity);

    if(_catStars.size() > _picStars.size())
        art::cutoff(_catStars,
                    _picStars.size());

    tri::cookTriangles(_picStars,
                       _tribox.data().picTriangles);
    tri::deleteEqual(_tribox.data().picTriangles,
                     _equalEps);

    tri::cookTriangles(_catStars,
                       _tribox.data().catTriangles);
    tri::deleteEqual(_tribox.data().catTriangles,
                     _equalEps);

    qDebug() << "triangles TOTAL:  " << _tribox.data().picTriangles.size()
             << " (pic)    " << _tribox.data().catTriangles.size() << " (cat)";

    tri::cookTriangleBox(_tribox.data(), _similarEps);

    qDebug() << "triangles SIMILAR:  " << _tribox.data().picTriangles.size()
             << " (pic)    " << _tribox.data().catTriangles.size() << " (cat)";

    tri::triangles2Artifacts(_tribox.data().picTriangles,
                             _tribox.data().catTriangles,
                             _equalEps,
                             _picStars,
                             _catStars);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::measureWork(int &xTarget,
                           int &yTarget)
{
    calcLinCor(_picStars,
               _catStars,
               _lincor);
    qDebug() << "a1 = " << _lincor.a1
             << "    b1 = " << _lincor.b1
             << "    c1 = " << _lincor.c1;
    qDebug() << "a2 = " << _lincor.a2
             << "    b2 = " << _lincor.b2
             << "    c2 = " << _lincor.c2;
    int x0 = xTarget;
    int y0 = yTarget;
    conversion(_lincor,
               xTarget, yTarget,
               xTarget, yTarget);
    int dx = xTarget - x0;
    int dy = yTarget - y0;
    qDebug() << "dx = " << dx
             << "    dy = " << dy;
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
