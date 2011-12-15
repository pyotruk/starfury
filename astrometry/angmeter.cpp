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
    _equatedStarQuantity = s->value(SKEY_EQUATED_STAR_QUANTITY, _defaultEquatedStarQuantity).toInt();
    _equalEps = s->value(SKEY_EQUAL_EPS, _defaultEqualEps).toDouble();
    _similarEps = s->value(SKEY_SIMILAR_EPS, _defaultSimilarEps).toDouble();
    _checkEps = s->value(SKEY_CHECK_EPS, _defaultCheckEps).toDouble();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::saveSettings(QSettings *s)
{
    s->setValue(SKEY_SCREEN_WIDTH, _screen.width());
    s->setValue(SKEY_SCREEN_HEIGHT, _screen.height());
    s->setValue(SKEY_MAX_STAR_QUANTITY, _maxStarQuantity);
    s->setValue(SKEY_EQUATED_STAR_QUANTITY, _equatedStarQuantity);
    s->setValue(SKEY_EQUAL_EPS, _equalEps);
    s->setValue(SKEY_SIMILAR_EPS, _similarEps);
    s->setValue(SKEY_CHECK_EPS, _checkEps);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::setScreenSize(const int width,
                             const int height)
{
    _screen.setWidth(width);
    _screen.setHeight(height);
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputScreenStars(ArtifactBox *a,
                                double xTarget,
                                double yTarget)
{
    a->lock().lockForRead();
    _picStars = a->data();
    a->lock().unlock();

    QTime t;
    t.start();

    _tribox.lock().lockForWrite();
    this->equation();
    _tribox.lock().unlock();

    LinCor cor;
    lincor::cook(_picStars,
                 _catStars,
                 cor);

    if(this->checkEquation(_picStars, _catStars, cor, _defaultCheckEps))
    {
        qDebug() << "a1 = " << cor.a1
                 << "    b1 = " << cor.b1
                 << "    c1 = " << cor.c1;
        qDebug() << "a2 = " << cor.a2
                 << "    b2 = " << cor.b2
                 << "    c2 = " << cor.c2;

        emit sendTriangles(&_tribox);
        Artifact target(xTarget, yTarget);
        this->correctTarget(cor, target);
        emit sendTarget(target.center().x(),
                        target.center().y());
    }

    qDebug() << "Angmeter equation delay: " << t.elapsed();
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
void Angmeter::correctTarget(const LinCor &cor,
                             Artifact &target)
{
    double xCat, yCat;
    lincor::conversion(cor,
                       target.center().x(),
                       target.center().y(),
                       xCat, yCat);
    double dx = xCat - target.center().x();
    double dy = yCat - target.center().y();
    qDebug() << "dx = " << dx
             << "    dy = " << dy;
    target.setCenter(QPoint(xCat, yCat));
}
/////////////////////////////////////////////////////////////////////////////////////
bool Angmeter::checkEquation(const ArtifactVector &picStars,
                             const ArtifactVector &catStars,
                             const LinCor &cor,
                             const double eps)
{
    if(picStars.size() < _equatedStarQuantity)    return false;
    if(catStars.size() < _equatedStarQuantity)    return false;
    double x, y;
    for(ArtifactVector::const_iterator itPic = picStars.constBegin(),
                                       itCat = catStars.constBegin();
        itPic < picStars.constEnd();
        ++itPic, ++itCat)
    {
        lincor::conversion(cor,
                           itPic->center().x(),
                           itPic->center().y(),
                           x,
                           y);
        if(!art::isEqual(*itCat, Artifact(x, y), eps))
        {
            return false;
        }
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
