
#include <QtGui>

class Downloader;
class QWebView;
QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QUrl& url);

protected slots:

    void adjustLocation();
    void changeLocation();
    void adjustTitle();
    void setProgress(int p);
    void finishLoading(bool);

    void viewSource();
    void slotSourceDownloaded();

    void showAbout();

private:
    Downloader *downloader;

    QWebView *view;
    QLineEdit *locationEdit;
    QAction *rotateAction;
    int progress;
};
