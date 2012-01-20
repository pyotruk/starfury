#ifndef REC_WND_H
#define REC_WND_H
/////////////////////////////////////////////////////////////////////////////////////
#include <QWidget>
#include <QSettings>
#include <QString>
/////////////////////////////////////////////////////////////////////////////////////
namespace rec_wnd
{
static const QString _skeyPrefGeometry("/gui/rec_wnd/geometry");
}
/////////////////////////////////////////////////////////////////////////////////////
class RecWindow : public QWidget
{
    Q_OBJECT
public:
    explicit RecWindow(const QString &title,
                       QSettings *s) : _settings(s)
    {
        this->setWindowTitle(title);
        _skeyGeometry = rec_wnd::_skeyPrefGeometry + title;
        this->restoreGeometry(s->value(_skeyGeometry).toByteArray());
    }

protected:
    void closeEvent(QCloseEvent *e)
    {
        _settings->setValue(_skeyGeometry, this->saveGeometry());
        QWidget::closeEvent(e);
    }

    virtual void drawAll() = 0;

    QSettings *_settings;

private:
    RecWindow(const RecWindow&) {}
    RecWindow& operator =(const RecWindow&) {return *this;}

    QString    _skeyGeometry;
};
/////////////////////////////////////////////////////////////////////////////////////
#endif // REC_WND_H
