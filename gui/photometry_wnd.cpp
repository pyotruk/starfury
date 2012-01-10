#include "photometry_wnd.h"
/////////////////////////////////////////////////////////////////////////////////////
PhotometryWindow::PhotometryWindow(QSettings *s,
                                   QWidget *parent) :
    QFrame(parent),
    _settings(s),
    _t0(timeutils::msecFromDayBegin() / 1000),
    _plot(new QwtPlot(this)),
    _layout(new QBoxLayout(QBoxLayout::LeftToRight, this)),
    _samples(0)
{
    this->loadSettings(s);

    this->setWindowTitle(__title);
    this->setPalette(QPalette(QBrush(__borderColor),
                              QBrush(__backgroundColor),
                              QBrush(__borderColor),
                              QBrush(__borderColor),
                              QBrush(__borderColor),
                              QBrush(__borderColor),
                              QBrush(__borderColor),
                              QBrush(__borderColor),
                              QBrush(__backgroundColor)));

    _layout->addWidget(_plot, 0);
    this->setLayout(_layout);
    this->resize(_width, _height);

    //axis
    _plot->setAxisAutoScale(QwtPlot::xBottom, false);
    _plot->setAxisScale(QwtPlot::xBottom, _minVal.x(), _maxVal.x());
    _plot->setAxisScale(QwtPlot::yLeft, _minVal.y(), _maxVal.y());

    //plot
    _plot->setAutoReplot(false);

    //curves
    _curves[0].setPen(QColor(__curveColor0));
    _curves[0].setStyle(QwtPlotCurve::Steps);
    _curves[0].attach(_plot);

    _curves[1].setPen(QColor(__curbeColor1));
    _curves[1].setStyle(QwtPlotCurve::Steps);
    _curves[1].attach(_plot);
}
/////////////////////////////////////////////////////////////////////////////////////
PhotometryWindow::~PhotometryWindow()
{
    this->saveSettings(_settings);
}
/////////////////////////////////////////////////////////////////////////////////////
void PhotometryWindow::loadSettings(QSettings *s)
{
    _maxVal = s->value(__skeyMaxVal, __defaultMaxVal).toPointF();
    _minVal = s->value(__skeyMinVal, __defaultMinVal).toPointF();
}
/////////////////////////////////////////////////////////////////////////////////////
void PhotometryWindow::saveSettings(QSettings *s)
{
    s->setValue(__skeyMaxVal, _maxVal);
    s->setValue(__skeyMinVal, _minVal);
}
/////////////////////////////////////////////////////////////////////////////////////
void PhotometryWindow::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    _curves[0].setSamples(_signal);
    _curves[1].setSamples(_fone);
    _plot->replot();
}
/////////////////////////////////////////////////////////////////////////////////////
void PhotometryWindow::addPoint(double timeMarker, //sec.000
                                double signal,
                                double fone)
{
    double t = timeMarker - _t0;
    if(t > _maxVal.x())
    {
        qDebug() << "series size: " << _signal.size();
        _signal.clear();
        _fone.clear();
        qDebug() << "               series cleared";
        _t0 = timeMarker;
        return;
    }
    _signal.push_back(QPointF(t, signal));
    _fone.push_back(QPointF(t, fone));

    ++_samples;
    if(_samples > _repaintSample)
    {
        _samples = 0;
        this->repaint();
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
