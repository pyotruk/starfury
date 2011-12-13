#include "starcatreader.h"
/////////////////////////////////////////////////////////////////////////////////////
StarcatReader::StarcatReader(QSettings *settings) :
    _settings(settings),
    _starVec0(new StarVector),
    _starVec1(new StarVector),
    _mutex(new QMutex)
{
    this->moveToThread(this);
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
    _path = settings->value(SKEY_STARCATREADER_PATH, DEFAULT_CATALOG_PATH).toString();
    _magnLim = settings->value(SKEY_STARCATREADER_MAGNLIM, _defaultMagnLim).toDouble();
    double fieldWidth = settings->value(SKEY_FIELD_WIDTH, _defaultFieldWidth).toDouble() / 60 * __deg2rad;
    double fieldHeight = settings->value(SKEY_FIELD_HEIGHT, _defaultFieldHeight).toDouble() / 60 * __deg2rad;
    double segmentSide = settings->value(SKEY_STARCATREADER_SEGMENTSIDE, _defaultSegmentSide).toDouble();
    double segmentEdge = settings->value(SKEY_STARCATREADER_SEGMENTEDGE, _defaultSegmentEdge).toDouble();
    _segment.setField(QSizeF(fieldWidth, fieldHeight));
    _segment.setSide(segmentSide);
    _segment.setEdge(segmentEdge);
}
/////////////////////////////////////////////////////////////////////////////////////
void StarcatReader::saveSettings(QSettings *settings)
{
    settings->setValue(SKEY_STARCATREADER_PATH, _path);
    settings->setValue(SKEY_STARCATREADER_MAGNLIM, _magnLim);
    settings->setValue(SKEY_FIELD_WIDTH, _segment.field().width() * __rad2deg * 60);
    settings->setValue(SKEY_FIELD_HEIGHT, _segment.field().height() * __rad2deg * 60);
    settings->setValue(SKEY_STARCATREADER_SEGMENTSIDE, _segment.side());
    settings->setValue(SKEY_STARCATREADER_SEGMENTEDGE, _segment.edge());
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
    qDebug() << "starcat work: " << time.elapsed() << " msec" << endl
             << "vec0: " << _starVec0->size() << endl
             << "vec1: " << _starVec1->size() << endl
             << "star: " << "a" << star.alpha()
             << " d" << star.delta()
             << " m" << star.magnitude() << endl;
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
    calcPositionRange(_file->size(),
                      _segment.rect().botLeft.delta,
                      _segment.rect().topRight.delta,
                      startPos,
                      finPos);
    _file->seek(startPos);
    Star star;
    uchar *data = new uchar[BYTES_PER_STAR];
    while(_file->pos() < finPos)
    {
        _file->read((char*)data, BYTES_PER_STAR);
        decodeStar(star, data);
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
StarVector* StarcatReader::stars()
{
    return _starVec1;
}
/////////////////////////////////////////////////////////////////////////////////////
SkySegment& StarcatReader::segment()
{
    return _segment;
}
/////////////////////////////////////////////////////////////////////////////////////
QMutex* StarcatReader::mutex()
{
    return _mutex;
}
/////////////////////////////////////////////////////////////////////////////////////
