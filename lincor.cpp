#include "lincor.h"
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void calcLinCor(const ArtifactVector &picStars,
                const ArtifactVector &catStars,
                LinCor &lincor)
{
    if(picStars.empty() || catStars.empty())    return;

    double Mxx, Mxy, Myy, Mx, My, Mxcx, Mxcy, Mxc, Mycx, Mycy, Myc,
           Sxx, Sxy, Syy, MMxcx, MMxcy, MMycx, MMycy, d;
    Mxx = 0;   Mxy = 0;   Myy = 0;   Mx = 0;   My = 0;   Mxcx = 0;
    Mxcy = 0;  Mxc = 0;   Mycx = 0;  Mycy = 0; Myc = 0;
    int n = picStars.size();

    for(ArtifactVector::const_iterator itPic = picStars.constBegin(),
                                       itCat = catStars.constBegin();
        itPic < picStars.constEnd();
        ++itPic, ++itCat)
    {
        Mxx  = Mxx  + itPic->center().x() * itPic->center().x();
        Mxy  = Mxy  + itPic->center().x() * itPic->center().y();
        Myy  = Myy  + itPic->center().y() * itPic->center().y();
        Mx   = Mx   + itPic->center().x();
        My   = My   + itPic->center().y();
        Mxcx = Mxcx + itCat->center().x() * itPic->center().x();
        Mxcy = Mxcy + itCat->center().x() * itPic->center().y();
        Mxc  = Mxc  + itCat->center().x();
        Mycx = Mycx + itCat->center().y() * itPic->center().x();
        Mycy = Mycy + itCat->center().y() * itPic->center().y();
        Myc  = Myc  + itCat->center().y();
    }
    Sxx = Mxx - Mx * Mx / n;
    Sxy = Mxy - Mx * My / n;
    Syy = Myy - My * My / n;
    MMxcx = Mxcx - Mxc * Mx / n;
    MMxcy = Mxcy - Mxc * My / n;
    MMycx = Mycx - Myc * Mx / n;
    MMycy = Mycy - Myc * My / n;
    d = Sxx * Syy - Sxy * Sxy;

    lincor.a1 = (MMxcx * Syy - MMxcy * Sxy) / d;
    lincor.b1 = (MMxcy * Sxx - MMxcx * Sxy) / d;
    lincor.c1 = (Mxc - lincor.a1 * Mx - lincor.b1 * My) / n;

    lincor.a2 = (MMycx * Syy - MMycy * Sxy) / d;
    lincor.b2 = (MMycy * Sxx - MMycx * Sxy) / d;
    lincor.c2 = (Myc - lincor.a2 * Mx - lincor.b2 * My) / n;
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
