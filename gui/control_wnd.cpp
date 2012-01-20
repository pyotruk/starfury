#include "control_wnd.h"
#include "ui_controlwindow.h"

/////////////////////////////////////////////////////////////////////////////////////
ControlWindow::ControlWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlWindow)
{
    ui->setupUi(this);

    //gui <--> gui connections
    connect(ui->sliderStrobSize, SIGNAL(sliderMoved(int)),
            this, SIGNAL(setStrobSize(int)));
    connect(ui->sliderAccum, SIGNAL(sliderMoved(int)),
            this, SIGNAL(setAccumCapacity(int)));
    connect(ui->checkBoxBin, SIGNAL(stateChanged(int)),
            this, SLOT(convertCheckBoxDetectionModeSignal(int)));

    connect(ui->radioBtnSimtri, SIGNAL(clicked()),
            this, SLOT(setSimtriAstrometryMethod()));
    connect(ui->radioBtnFreevec, SIGNAL(clicked()),
            this, SLOT(setFreevecAstrometryMethod()));

    connect(ui->radioBtnStars, SIGNAL(clicked()),
            this, SLOT(setStarDetectionMode()));
    connect(ui->radioBtnTargets, SIGNAL(clicked()),
            this, SLOT(setTargetDetectionMode()));

    connect(ui->sliderStrobSize, SIGNAL(sliderMoved(int)),
            this, SLOT(updateSliderLabels()));
    connect(ui->sliderAccum, SIGNAL(sliderMoved(int)),
            this, SLOT(updateSliderLabels()));
}
/////////////////////////////////////////////////////////////////////////////////////
ControlWindow::~ControlWindow()
{
    delete ui;
}
/////////////////////////////////////////////////////////////////////////////////////
void ControlWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    emit closed();
}
/////////////////////////////////////////////////////////////////////////////////////
void ControlWindow::initFace(const int strobSize,
                             const astrometry::METHOD method,
                             const int accumCapacity,
                             const Detector::MODE mode)
{
    ui->sliderStrobSize->setValue(strobSize);
    ui->sliderAccum->setValue(accumCapacity);
    this->updateSliderLabels();

    switch(method)
    {
    case astrometry::SIMTRI:
        ui->radioBtnSimtri->setChecked(true);
        break;
    case astrometry::FREEVEC:
        ui->radioBtnFreevec->setChecked(true);
        break;
    }

    switch(mode)
    {
    case Detector::STAR_DETECTION:
        ui->radioBtnStars->setChecked(true);
        break;
    case Detector::TARGET_DETECTION:
        ui->radioBtnTargets->setChecked(true);
        break;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void ControlWindow::updateSliderLabels()
{
    QVariant content;
    content = ui->sliderStrobSize->value();
    ui->labelStrobSize->setText(content.toString());
    content = ui->sliderAccum->value();
    ui->labelAccum->setText(content.toString());
}
/////////////////////////////////////////////////////////////////////////////////////
void ControlWindow::inputMeasureError(double errAlpha,  //rad
                                      double errDelta)  //rad
{
    ui->labelAlpha->setText("alpha  " +
                              QString::number(errAlpha * __rad2deg * 3600));
    ui->labelDelta->setText("delta  " +
                            QString::number(errDelta * __rad2deg * 3600));
}
/////////////////////////////////////////////////////////////////////////////////////
void ControlWindow::convertCheckBoxDetectionModeSignal(int state)
{
    switch(state)
    {
    case Qt::Checked:
        emit setBinEnabled(true);
        break;
    case Qt::Unchecked:
        emit setBinEnabled(false);
        break;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

