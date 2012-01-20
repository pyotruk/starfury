#include "detector_wnd.h"
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWindow::drawAll()
{
    ImageWindow::drawAll();

    switch(_mode)
    {
    case Detector::STAR_DETECTION:
        draw::picStarsMarks(_img, _picStars, __picStarsColor, __picStarsPen);
        draw::catStarsMarks(_img, _catStars, __catStarsColor, __catStarsPen);
        draw::starConfig(_img, _eqPicStars, __eqPicStarsColor, __eqPicStarsPen);
        _eqPicStars.clear();
        draw::starConfig(_img, _eqCatStars, __eqCatStarsColor, __eqCatStarsPen);
        _eqCatStars.clear();
        break;

    case Detector::TARGET_DETECTION:
        draw::targetMarks(this->_img, _targets, __targetsColor, __targetsPen);
        break;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWindow::inputPicStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _picStars = a->data();
    a->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWindow::inputCatStars(ArtifactBox *a)
{
    a->lock().lockForRead();
    _catStars = a->data();
    a->lock().unlock();

    draw::convertStarMagn(_catStars);
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWindow::inputEquatedStars(ArtifactBox *pic,
                                       ArtifactBox *cat)
{
    pic->lock().lockForRead();
    cat->lock().lockForRead();
    _eqPicStars = pic->data();
    _eqCatStars = cat->data();
    cat->lock().unlock();
    pic->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWindow::inputTargets(ArtifactBox *t)
{
    t->lock().lockForRead();
    _targets = t->data();
    t->lock().unlock();
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWindow::clear()
{
    _targets.clear();
    _picStars.clear();
    _catStars.clear();
    _eqPicStars.clear();
    _eqCatStars.clear();
}
/////////////////////////////////////////////////////////////////////////////////////
void DetectorWindow::setMode(int m)
{
    _mode = (Detector::MODE)m;
    this->clear();
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

