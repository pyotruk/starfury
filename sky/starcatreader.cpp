#include "starcatreader.h"
/////////////////////////////////////////////////////////////////////////////////////
StarcatReader::StarcatReader(QSettings *settings) :
    _settings(settings),
    _starVec0(new StarVector),
    _starVec1(new StarVector),
    _mutex(new QMutex)
{
    this->moveToThread(this);
    qDebug() << "StarcatReader :" << this->thread();
    this->loadSettings(_settings);
    _file = new QFile(_path);
    if(!_file->open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot open file for reading: "
                 << qPrintable(_file->errorString());
    }
}
/////////////////////////////////////////////////////////////////////////////////////
StarcatReader::~StarcatReader()
{
    if(!this->wait(_termTimeout))   this->terminate();
    this->saveSettings(_settings);
    delete _mutex;
    _file->close();
    delete _file;
    delete _starVec0;
    delete _starVec1;
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatReader::loadSettings(QSettings *settings)
{
    _path = settings->value(__skeyStarcatPath, __defaultStarcatPath).toString();
    _magnLim = settings->value(__skeyStarcatMagnLim, _defaultMagnLim).toDouble();
    double fieldWidth = settings->value(__skeyFieldWidth, __defaultFieldWidth).toDouble() * ac::_dmin2rad;
    double fieldHeight = settings->value(__skeyFieldHeight, __defaultFieldHeight).toDouble() * ac::_dmin2rad;
    double segmentSide = settings->value(__skeyStarcatSegmentSide, _defaultSegmentSide).toDouble();
    double segmentEdge = settings->value(__skeyStarcatSegmentEdge, _defaultSegmentEdge).toDouble();
    _segment.setField(QSizeF(fieldWidth, fieldHeight));
    _segment.setSide(segmentSide);
    _segment.setEdge(segmentEdge);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatReader::saveSettings(QSettings *settings)
{
    settings->setValue(__skeyStarcatPath, _path);
    settings->setValue(__skeyStarcatMagnLim, _magnLim);
    settings->setValue(__skeyFieldWidth, _segment.field().width() * ac::_rad2dmin);
    settings->setValue(__skeyFieldHeight, _segment.field().height() * ac::_rad2dmin);
    settings->setValue(__skeyStarcatSegmentSide, _segment.side());
    settings->setValue(__skeyStarcatSegmentEdge, _segment.edge());
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatReader::run()
{
    QTime time;
    time.start();

    this->readNewSegment();
    if(_mutex->tryLock(_timeout))
    {
        this->switchBuffers();
        _mutex->unlock();
    }
    _starVec0->clear();

    Star star;
    if(!_starVec1->isEmpty())     star = _starVec1->at(0);
    qDebug() << "StarcatReader work: " << time.elapsed() << " msec" << "\n"
             << "              vec0: " << _starVec0->size() << "    vec1: " << _starVec1->size();
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatReader::refresh(const double alpha,
                            const double delta/*,
                            const double fieldWidth,
                            const double fieldHeight*/)
{
    if(_segment.isOnEdge(alpha, delta))
    {
        _segment.generateNew(alpha,
                             delta/*,
                             fieldWidth,
                             fieldHeight*/);
        this->start(QThread::NormalPriority);
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatReader::switchBuffers()
{
    StarVector *buf = _starVec0;
    _starVec0 = _starVec1;
    _starVec1 = buf;
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatReader::readNewSegment()
{
    qint64 startPos, finPos;
    guidecat::calcPositionRange(_file->size(),
                                _segment.rect().botLeft.delta,
                                _segment.rect().topRight.delta,
                                startPos,
                                finPos);
    _file->seek(startPos);
    Star star;
    uchar *data = new uchar[__bytesPerStar];
    while(_file->pos() < finPos)
    {
        _file->read((char*)data, __bytesPerStar);
        guidecat::decodeStar(star, data);
        if(star.magnitude() < _magnLim)
        {
            if(_segment.isBelong(star.alpha(), star.delta()))
            {
                _starVec0->push_back(star);
            }
        }
    }
    delete []data;
    qSort(*_starVec0);
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
