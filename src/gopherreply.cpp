
#include <QTextDocument>
#include <QtNetwork>
#include <QTcpSocket>

#include "gopherreply.hpp"

//! [constructor]
GopherReply::GopherReply(const QUrl &url)
    : QNetworkReply()
{
    sock = new QTcpSocket(0);
    connect(sock, SIGNAL(connected()), this, SLOT(sockConnected()));

/*    ftp = new QFtp(this);
    connect(ftp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(processListInfo(QUrlInfo)));
    connect(ftp, SIGNAL(readyRead()), this, SLOT(processData()));
    connect(ftp, SIGNAL(commandFinished(int, bool)), this, SLOT(processCommand(int, bool)));
*/
    offset = 0;
    units = QStringList() << tr("bytes") << tr("K") << tr("M") << tr("G")
                          << tr("Ti") << tr("Pi") << tr("Ei") << tr("Zi")
                          << tr("Yi");

    setUrl(url);
/*    ftp->connectToHost(url.host());
*/
    sock->connectToHost(url.host(), url.port(70));
    
    
    //sockConnected();
}
//! [constructor]

void GopherReply::sockConnected()
{
    if (!sock->waitForConnected(5000)) {
        //emit error(sock->error(), sock->errorString());
        qDebug() << "error";
        return;
    }
    QString ret = "\r\n";
    QString selector = url().path().right(url().path().size() - 2);
    if (selector == "/")
        selector = "";

    sock->write(selector.toAscii());
    sock->write(ret.toAscii());
    QString type = url().path().left(2).right(1);

    QString data = "";
    while (sock->state() == QAbstractSocket::ConnectedState)
    while (sock->waitForReadyRead(5000))
    {
        while (sock->canReadLine())
        {
            data += sock->readLine();
        }
    }

    
    if (type == "0")
    {
        //renderTxt(data);
        this->content = data.toLatin1();

        open(ReadOnly | Unbuffered);

        setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain; charset=iso-8859-1"));
        setHeader(QNetworkRequest::ContentLengthHeader, QVariant(this->content.size()));
        emit readyRead();
        emit finished();
        return;
    }




    QUrl u = url();
    if (!u.path().endsWith("/"))
        u.setPath(u.path() + "/");

    QString base_url = url().toString();
    QString base_path = u.path();





    //content += data;

    QString content = QString("<ol>");
    QString title = "TYPE = " + type + " | " + Qt::escape(base_url);
    QStringList main = data.split("\r\n");
    for (int i = 0; i < main.size(); i++)
    {
        QString dataline = main.at(i);
        QStringList list1 = dataline.split("\t");
        
        if (list1.size() == 0)
        {
            content += QString("<li>***ERROR***</li>\n");
        }
        else if (dataline == ".")
        {
            content += QString("<li>***END OK***</li>\n");
        }
        else
        {
            QStringListIterator javaStyleIterator(list1);
            QString type;
            QString username;
            if (javaStyleIterator.hasNext())
            {
                QString first = javaStyleIterator.next();
                type = first.left(1);
                username = first.right(first.size() - 1);
            }
            QString selector = "";
            if (javaStyleIterator.hasNext())
            {
                selector = javaStyleIterator.next();
            }
            if (selector == "TITLE")
                title = QString(username);
            QString host = "";
            if (javaStyleIterator.hasNext())
            {
                host = javaStyleIterator.next();
            }
            QString port = "";
            if (javaStyleIterator.hasNext())
            {
                port = javaStyleIterator.next();
            }

            //QString type = list1.at(0).left(1);
            if (type == "i") {
                content += QString("<li class=\"typei\">" + username + "</li>\n");
            }
            else
            {
                content += QString("<li ");
                /*if (type == "0")
                    content += QString("<img href=\"file://cog.png\" alt=\"TEXT\" />");
                else (type == "1")
                    content += QString("<img href=\"file://cog.png\" alt=\"MENU\" />");
                else (type == "7")
                    content += QString("<img href=\"file://cog.png\" alt=\"INDEX\" />");
                else
                    content += QString("<img href=\"file://cog.png\" alt=\"BINARY\" />");*/
                content += QString("class=\"type" + type + "\">");
                
                content += QString("<a href=\"gopher://" + host + ":" + port + "/");
                content += type;
                content += selector;
                content += QString("\">");
                
                if (type == "1")
                    content += QString("/");
                
                content += username;
                content += QString("</a></li>\n");
            }
        }
    }
    content += QString("  </ol>");
    
    content += QString("</body>\n"
                       "</html>\n");

    content = QString(
        "<html>\n"
        "<head>\n"
        "  <title>" + title + "</title>\n"
        "  <style type=\"text/css\">\n"
        "ol {\n"
        "font-family: Monospace;\n"
        "font-size: medium;\n"
        "white-space: pre;\n"
        "}\n"
        "li { list-style-type: none; }\n"
        "a { text-decoration: none; }\n"
        "a:hover { background: #eee; }\n"
        "li.type1 { list-style: url('data:image/gif;base64,R0lGODdhFAAXAPAAMf///wAAACwAAAAAFAAXAAACOYSPqcvtD1OYNAZ1XcZtL1oZHpSNJGBqoYVO0QG+aJt263mTuUbL9AWcBYctoZEIdP1AzBisCc0UAAA7'); }\n"
        "li { list-style: url('data:image/gif;base64,R0lGODdhFAAXAPAAMf///wAAACwAAAAAFAAXAAACOYSPqcvtD02YtFYV4Ez55S5J1gZ0INicoWixVBmNpry80WG7JLfftozC9WK9UW2oIQGPwSTw+YI+CwA7'); }\n"
        "li.typei { list-style: none; }\n"
        "li.type7 { list-style: url('data:image/gif;base64,R0lGODdhFAAXAPAAMf///wAAACwAAAAAFAAXAAACQoSPqcvtD02YtNoKQkRUWT8l2hFypSSNZ7ZqoWqOJJzFjCqzOZmKK8oC7Wq/1FC36AiTRdxShPQdldLN7ILF6rLZAgA7'); }\n"
        "li.typeh { list-style: url('data:image/gif;base64,R0lGODdhFAAXAPAAMf///wAAACwAAAAAFAAXAAACQISPqcvtD02YtFYV4Ey5g8x9iOeFovRd5gmWnHWO22HNo0Nh4JKvvE0DynY/Yo/RChJ1RqFpdywOI8OatZa6agsAOw%3D%3D'); }\n"
        "  </style>\n"
        "</head>\n\n"
        "<body>\n") + content;

    this->content = content.toLatin1();
    open(ReadOnly | Unbuffered);

    setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/html; charset=iso-8859-1"));
    setHeader(QNetworkRequest::ContentLengthHeader, QVariant(this->content.size()));
    emit readyRead();
    emit finished();

    //sock->close();
}

//! [process command]
void GopherReply::processCommand(int, bool err)
{/*
    if (err) {
        setError(ContentNotFoundError, tr("Unknown command"));
        emit error(ContentNotFoundError);
        return;
    }

    switch (ftp->currentCommand()) {
    case QFtp::ConnectToHost:
        ftp->login();
        break;

    case QFtp::Login:
        ftp->list(url().path());
        break;

    case QFtp::List:
        if (items.size() == 1)
            ftp->get(url().path());
        else
            setListContent();
        break;

    case QFtp::Get:
        setContent();

    default:
        ;
    }*/
}
//! [process command]

//! [process list info]
void GopherReply::processListInfo(const QUrlInfo &urlInfo)
{
    items.append(urlInfo);
}
//! [process list info]

//! [process data]
void GopherReply::processData()
{
    content += ftp->readAll();
}
//! [process data]

//! [set content]
void GopherReply::setContent()
{
    open(ReadOnly | Unbuffered);
    setHeader(QNetworkRequest::ContentLengthHeader, QVariant(content.size()));
    emit readyRead();
    emit finished();
    ftp->close();
}
//! [set content]

//! [set list content]
void GopherReply::setListContent()
{
    QUrl u = url();
    if (!u.path().endsWith("/"))
        u.setPath(u.path() + "/");

    QString base_url = url().toString();
    QString base_path = u.path();

    open(ReadOnly | Unbuffered);
    QString content(
        "<html>\n"
        "<head>\n"
        "  <title>" + Qt::escape(base_url) + "</title>\n"
        "  <style type=\"text/css\">\n"
        "  th { background-color: #aaaaaa; color: black }\n"
        "  table { border: solid 1px #aaaaaa }\n"
        "  tr.odd { background-color: #dddddd; color: black\n }\n"
        "  tr.even { background-color: white; color: black\n }\n"
        "  </style>\n"
        "</head>\n\n"
        "<body>\n"
        "<h1>" + tr("Listing for %1").arg(base_path) + "</h1>\n\n"
        "<table align=\"center\" cellspacing=\"0\" width=\"90%\">\n"
        "<tr><th>Name</th><th>Size</th></tr>\n");

    QUrl parent = u.resolved(QUrl(".."));

    if (parent.isParentOf(u))

        content += QString("<tr><td><strong><a href=\"" + parent.toString() + "\">"
            + tr("Parent directory") + "</a></strong></td><td></td></tr>\n");

    int i = 0;
    foreach (const QUrlInfo &item, items) {

        QUrl child = u.resolved(QUrl(item.name()));

        if (i == 0)
            content += QString("<tr class=\"odd\">");
        else
            content += QString("<tr class=\"even\">");

        content += QString("<td><a href=\"" + child.toString() + "\">"
                           + Qt::escape(item.name()) + "</a></td>");

        qint64 size = item.size();
        int unit = 0;
        while (size) {
            qint64 new_size = size/1024;
            if (new_size && unit < units.size()) {
                size = new_size;
                unit += 1;
            } else
                break;
        }

        if (item.isFile())
            content += QString("<td>" + QString::number(size) + " "
                               + units[unit] + "</td></tr>\n");
        else
            content += QString("<td></td></tr>\n");

        i = 1 - i;
    }

    content += QString("</table>\n"
                       "</body>\n"
                       "</html>\n");

    this->content = content.toUtf8();

    setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/html; charset=UTF-8"));
    setHeader(QNetworkRequest::ContentLengthHeader, QVariant(this->content.size()));
    emit readyRead();
    emit finished();
    ftp->close();
    
    //sock->close();
}
//! [set list content]

// QIODevice methods

//! [abort]
void GopherReply::abort()
{
}
//! [abort]

//! [bytes available]
qint64 GopherReply::bytesAvailable() const
{
    return content.size() - offset;
}
//! [bytes available]

//! [is sequential]
bool GopherReply::isSequential() const
{
    return true;
}
//! [is sequential]

//! [read data]
qint64 GopherReply::readData(char *data, qint64 maxSize)
{
    if (offset < content.size()) {
        qint64 number = qMin(maxSize, content.size() - offset);
        memcpy(data, content.constData() + offset, number);
        offset += number;
        return number;
    } else
        return -1;
}
//! [read data]
