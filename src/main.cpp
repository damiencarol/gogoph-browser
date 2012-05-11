
#include <QtGui>

#include "mainwindow.hpp"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QUrl url;
    if (argc > 1)
        url = QUrl(argv[1]);
    else
        url = QUrl("gopher://dams.zapto.org");

    MainWindow *browser = new MainWindow(url);
    #if defined Q_OS_SYMBIAN || defined Q_WS_HILDON || defined Q_WS_MAEMO_5 || defined Q_WS_SIMULATOR
        browser->showMaximized();
    #else
        browser->show();
    #endif
    return app.exec();
}
