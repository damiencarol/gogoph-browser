
#include <QtGui>
#include <QtNetwork>
#include <QtWebKit>

#include "mainwindow.hpp"
#include "networkaccessmanager.hpp"
#include "downloader.hpp"

//! [1]

MainWindow::MainWindow(const QUrl& url)
{
    progress = 0;

/*    QFile file;
    file.setFileName(":/jquery.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    file.close();*/
//! [1]

    QNetworkProxyFactory::setUseSystemConfiguration(true);

//! [2]
    view = new QWebView(this);
    
    QNetworkAccessManager *oldManager = view->page()->networkAccessManager();
    NetworkAccessManager *newManager = new NetworkAccessManager(oldManager, view);
    view->page()->setNetworkAccessManager(newManager);

    view->page()->setForwardUnsupportedContent(true);

    downloader = new Downloader(this, newManager);

    connect(view->page(), SIGNAL(unsupportedContent(QNetworkReply *)),
            downloader, SLOT(saveFile(QNetworkReply *)));
    connect(view->page(), SIGNAL(downloadRequested(const QNetworkRequest &)),
            downloader, SLOT(startDownload(const QNetworkRequest &)));


    //view->load(url);
    connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));






    locationEdit = new QLineEdit(this);
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(view->pageAction(QWebPage::Back));
    toolBar->addAction(view->pageAction(QWebPage::Forward));
    toolBar->addAction(view->pageAction(QWebPage::Reload));
    toolBar->addAction(view->pageAction(QWebPage::Stop));
    toolBar->addWidget(locationEdit);
//! [2]

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    QAction* viewSourceAction = new QAction("Page Source", this);
    connect(viewSourceAction, SIGNAL(triggered()), SLOT(viewSource()));
    viewMenu->addAction(viewSourceAction);

//! [3]
    QMenu *effectMenu = menuBar()->addMenu(tr("&Effect"));
    effectMenu->addAction("Highlight all links", this, SLOT(highlightAllLinks()));

    rotateAction = new QAction(this);
    rotateAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    rotateAction->setCheckable(true);
    rotateAction->setText(tr("Turn images upside down"));
    connect(rotateAction, SIGNAL(toggled(bool)), this, SLOT(rotateImages(bool)));
    effectMenu->addAction(rotateAction);

    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
    /*toolsMenu->addAction(tr("Remove GIF images"), this, SLOT(removeGifImages()));
    toolsMenu->addAction(tr("Remove all inline frames"), this, SLOT(removeInlineFrames()));
    toolsMenu->addAction(tr("Remove all object elements"), this, SLOT(removeObjectElements()));
    toolsMenu->addAction(tr("Remove all embedded elements"), this, SLOT(removeEmbeddedElements()));*/

    // Add Menu '?'
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About GOGOPH Browser"), this, SLOT(showAbout()));


    setCentralWidget(view);
    setUnifiedTitleAndToolBarOnMac(true);

}
//! [3]

void MainWindow::viewSource()
{
    QNetworkAccessManager* accessManager = view->page()->networkAccessManager();
    QNetworkRequest request(view->url());
    QNetworkReply* reply = accessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(slotSourceDownloaded()));
}

void MainWindow::slotSourceDownloaded()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(const_cast<QObject*>(sender()));
    QTextEdit* textEdit = new QTextEdit(NULL);
    textEdit->setAttribute(Qt::WA_DeleteOnClose);
    textEdit->show();
    textEdit->setPlainText(reply->readAll());
    reply->deleteLater();
}

//! [4]
void MainWindow::adjustLocation()
{
    locationEdit->setText(view->url().toString());
}

void MainWindow::changeLocation()
{
    QString urlStr = locationEdit->text();
    QUrl url = QUrl(urlStr);
    if (url.scheme() == "")
        url = "gopher://" + url.toString();

    if (url.path() == "")
        url.setPath("/1/");
    if (url.path() == "/")
        url.setPath("/1/");
    if (url.path() == "/1")
        url.setPath("/1/");


    view->load(url);
    view->setFocus();
    /*
    QNetworkAccessManager* accessManager = view->page()->networkAccessManager();
    QNetworkRequest request(url);
    QNetworkReply* reply = accessManager->get(request);
    view->setHtml(reply->readAll());*/
}
//! [4]

//! [5]
void MainWindow::adjustTitle()
{
    if (progress <= 0 || progress >= 100)
        setWindowTitle(view->title());
    else
        setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
}

void MainWindow::setProgress(int p)
{
    progress = p;
    adjustTitle();
}
//! [5]

void MainWindow::finishLoading(bool)
{
    progress = 100;
    adjustTitle();
}

void MainWindow::showAbout()
{
    //QString code = "$('embed').remove()";
    //view->page()->mainFrame()->evaluateJavaScript(code);
}
