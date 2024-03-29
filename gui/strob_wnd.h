#ifndef STROB_WND_H
#define STROB_WND_H
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#include "image_wnd.h"
/////////////////////////////////////////////////////////////////////////////////////
class StrobWindow : public ImageWindow
{
    Q_OBJECT
public:
    explicit StrobWindow(const QString &title,
                         QSettings *s)
        : ImageWindow(title, s) {}
private:
    void drawAll() {ImageWindow::drawAll();}
};
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif // STROB_WND_H
