
#ifndef GOPHERREPLY_H
#define GOPHERREPLY_H

#include <QNetworkReply>
#include <QStringList>
#include <QUrlInfo>

QT_BEGIN_NAMESPACE
class QFtp;
class QTcpSocket;
QT_END_NAMESPACE

//! [class definition]
class GopherReply : public QNetworkReply
{
    Q_OBJECT

public:
    GopherReply(const QUrl &url);
    void abort();
    qint64 bytesAvailable() const;
    bool isSequential() const;

protected:
    qint64 readData(char *data, qint64 maxSize);

private slots:
    void sockConnected();
    void processCommand(int command, bool error);
    void processListInfo(const QUrlInfo &urlInfo);
    void processData();

private:
    void setContent();
    void setListContent();


	QTcpSocket *sock;

    QFtp *ftp;
    QList<QUrlInfo> items;
    QByteArray content;
    qint64 offset;
    QStringList units;
};
//! [class definition]

#endif
