
#include <QtNetwork>

#include "networkaccessmanager.hpp"
#include "ftpreply.hpp"
#include "gopherreply.hpp"

NetworkAccessManager::NetworkAccessManager(QNetworkAccessManager *manager, QObject *parent)
    : QNetworkAccessManager(parent)
{
    setCache(manager->cache());
    setCookieJar(manager->cookieJar());
    setProxy(manager->proxy());
    setProxyFactory(manager->proxyFactory());
}

//! [create request]
QNetworkReply *NetworkAccessManager::createRequest(
    QNetworkAccessManager::Operation operation, const QNetworkRequest &request,
    QIODevice *device)
{
    if (request.url().scheme() == "")
        // Handle DNS as gopher protocol by default.
        return new GopherReply(request.url());

    if (request.url().scheme() == "gopher")
        // Handle gopher:// URLs separately by creating custom QNetworkReply
        // objects.
        return new GopherReply(request.url());

    if (request.url().scheme() != "ftp")
        return QNetworkAccessManager::createRequest(operation, request, device);

    if (operation == GetOperation)
        // Handle ftp:// URLs separately by creating custom QNetworkReply
        // objects.
        return new FtpReply(request.url());
    else
        return QNetworkAccessManager::createRequest(operation, request, device);
}
//! [create request]
