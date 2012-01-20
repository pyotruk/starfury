#ifndef PHOTOMETRY_WND_H
#define PHOTOMETRY_WND_H
/////////////////////////////////////////////////////////////////////////////////////
#include <qwt_scale_map.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot.h>
#include <qwt_math.h>
/////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
#include <QColor>
#include <QVector>
#include <QPointF>
#include <QLayout>
#include <QFont>
/////////////////////////////////////////////////////////////////////////////////////
#include "gui/rec_wnd.h"
#include "utils/timeutils.h"
/////////////////////////////////////////////////////////////////////////////////////
//setting keys
static const QString __skeyMaxVal("/gui/Photometry/MaxVal");
static const QString __skeyMinVal("/gui/Photometry/MinVal");
/////////////////////////////////////////////////////////////////////////////////////
//default settings
static const QPointF __defaultMaxVal(120, 255);
static const QPointF __defaultMinVal(0, 0);
/////////////////////////////////////////////////////////////////////////////////////
static const QColor  __backgroundColor(53, 62, 102);
static const QColor  __borderColor(210, 185, 109);
static const QColor  __curveColor0(Qt::green);
static const QColor  __curbeColor1(Qt::white);
/////////////////////////////////////////////////////////////////////////////////////
class PhotometryWindow : public RecWindow
{
    Q_OBJECT
public:
    explicit PhotometryWindow(const QString &title,
                              QSettings *s);
    ~PhotometryWindow();
public slots:
    void addPoint(double timeMarker, //sec.000
                  double signal,
                  double fone);
protected:
    virtual void paintEvent(QPaintEvent *);
    void drawAll();
private:
    static const int _curveCount = 2;
    static const int _repaintSample = 0; //samples
    double           _t0;
    QVector<QPointF> _signal;
    QVector<QPointF> _fone;
    QwtPlotCurve     _curves[_curveCount];
    QwtPlot         *_plot;
    QBoxLayout      *_layout;
    int              _samples;
    QPointF          _maxVal;
    QPointF          _minVal;
    void loadSettings(QSettings*);
    void saveSettings(QSettings*);
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // PHOTOMETRY_WND_H
