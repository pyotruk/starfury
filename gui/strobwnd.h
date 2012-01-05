#ifndef STROBWND_H
#define STROBWND_H
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#include "imagewindow.h"
#include "boxes/artifact.h"
#include "drawing.h"
/////////////////////////////////////////////////////////////////////////////////////
class StrobWindow : public ImageWindow
{
    Q_OBJECT
public:
    explicit StrobWindow(const QString &title,
                         const QPoint  &pos,
                         QWidget *parent = 0)
        : ImageWindow(title, pos, parent) {}
    ~StrobWindow() {}
private:
    ArtifactBox     _artifactBox;
    ArtifactBox     _starBox;
    ArtifactVector  _eqPicStars;
    ArtifactVector  _eqCatStars;
    void drawAll();
private slots:
    void inputScreenStars(ArtifactBox*);
    void inputCatStars(ArtifactBox*);
    void inputEquatedStars(ArtifactBox *pic,
                           ArtifactBox *cat);
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROBWND_H
