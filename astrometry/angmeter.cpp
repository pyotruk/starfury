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
    int screenWidth = s->value(__skeyScreenWidth, _defaultScreenWidth).toInt();
    int screenHeight = s->value(__skeyScreenHeight, _defaultScreenHeight).toInt();
    this->setScreenSize(screenWidth, screenHeight);
    _maxStarQuantity = s->value(__skeyMaxStarQuantity, _defaultMaxStarQuantity).toInt();
    _equatedStarQuantity = s->value(__skeyEquatedStarQuantity, _defaultEquatedStarQuantity).toInt();
    _equalEps = s->value(__skeyEqualEps, _defaultEqualEps).toDouble();
    _similarEps = s->value(__skeySimilarEps, _defaultSimilarEps).toDouble();
    _checkEps = s->value(__skeyCheckEps, _defaultCheckEps).toDouble();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::saveSettings(QSettings *s)
{
    s->setValue(__skeyScreenWidth, _screen.width());
    s->setValue(__skeyScreenHeight, _screen.height());
    s->setValue(__skeyMaxStarQuantity, _maxStarQuantity);
    s->setValue(__skeyEquatedStarQuantity, _equatedStarQuantity);
    s->setValue(__skeyEqualEps, _equalEps);
    s->setValue(__skeySimilarEps, _similarEps);
    s->setValue(__skeyCheckEps, _checkEps);
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
    _picStars = *a;
    a->lock().unlock();

    QTime t;
    t.start();

    _tribox.lock().lockForWrite();
    this->equation();
    _tribox.lock().unlock();

    LinCor cor;
    lincor::cook(_picStars.data(),
                 _catStars.data(),
                 cor);

    if(this->checkEquation(_picStars.data(),
                           _catStars.data(),
                           cor,
                           _defaultCheckEps))
    {
        qDebug() << "a1 = " << cor.a1
                 << "    b1 = " << cor.b1
                 << "    c1 = " << cor.c1;
        qDebug() << "a2 = " << cor.a2
                 << "    b2 = " << cor.b2
                 << "    c2 = " << cor.c2;

        //emit sendTriangles(&_tribox);

        _eqPicStars.lock().lockForWrite();
        _eqCatStars.lock().lockForWrite();
        _eqPicStars = _picStars;
        _eqCatStars = _catStars;
        _eqCatStars.lock().unlock();
        _eqPicStars.lock().unlock();

        emit sendEquatedStars(&_eqPicStars, &_eqCatStars);


        _target.lock().lockForWrite();
        _target.data().setCenter(QPointF(xTarget, yTarget));
        this->correctTarget(cor, _target.data());
        _target.setTimeMarker(_picStars.timeMarker());
        _target.lock().unlock();

        emit sendTarget(&_target);
    }

    qDebug() << "Angmeter equation delay: " << t.elapsed();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::inputCatStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _catStars = *a;
    a->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void Angmeter::equation()
{
    art::selectOnCircle(_picStars.data(),
                        QPoint(_screen.width() / 2, _screen.height() / 2),
                        _screen.height() / 2);
//    art::cutoff(_picStars.data(),
//                _maxStarQuantity);

    art::selectOnCircle(_catStars.data(),
                        QPoint(_screen.width() / 2, _screen.height() / 2),
                        _screen.height() / 2);
//    art::cutoff(_catStars.data(),
//                _maxStarQuantity);

//    if(_catStars.data().size() > _picStars.data().size())
//        art::cutoff(_catStars.data(),
//                    _picStars.data().size());

    if(_picStars.data().size() > _catStars.data().size())
    {
        art::cutoff(_picStars.data(), _catStars.data().size());
    }


//    tri::cookTriangles(_picStars.data(),
//                       _tribox.data().picTriangles);
//    tri::deleteEqual(_tribox.data().picTriangles,
//                     _equalEps);

//    tri::cookTriangles(_catStars.data(),
//                       _tribox.data().catTriangles);
//    tri::deleteEqual(_tribox.data().catTriangles,
//                     _equalEps);

//    qDebug() << "triangles TOTAL:  " << _tribox.data().picTriangles.size()
//             << " (pic)    " << _tribox.data().catTriangles.size() << " (cat)";

//    tri::cookTriangleBox(_tribox.data(), _similarEps);

//    qDebug() << "triangles SIMILAR:  " << _tribox.data().picTriangles.size()
//             << " (pic)    " << _tribox.data().catTriangles.size() << " (cat)";

//    tri::triangles2Artifacts(_tribox.data().picTriangles,
//                             _tribox.data().catTriangles,
//                             _equalEps,
//                             _picStars.data(),
//                             _catStars.data());

    id::equate(_picStars.data(),
               _catStars.data(),
               _similarEps,
               _picStars.data(),
               _catStars.data());
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
