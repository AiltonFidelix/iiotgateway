#include "interface.h"

#include <QTcpServer>
#include <QJsonDocument>
#include <QJsonObject>

ControlServer::ControlServer(QObject *parent)
    : QObject(parent)
{

}

ControlServer::~ControlServer()
{
    auto servers = m_httpServer.servers();
    qDeleteAll(servers);
}

bool
ControlServer::start(int port)
{
    registerRoutes();

    auto tcpserver = new QTcpServer();

    if (!tcpserver->listen(QHostAddress::Any, port) || !m_httpServer.bind(tcpserver))
    {
        delete tcpserver;
        return false;
    }

    qDebug() << "Listening on port" << tcpserver->serverPort();

    return true;
}

void
ControlServer::registerRoutes()
{
    auto index = [](const QHttpServerRequest &request)
    {
        qDebug() << request.method() << request.headers();
        QString response = "<h1>IIoTGateway settings server</h1>";
        return QHttpServerResponse(response);
    };

    auto settings = [](const QHttpServerRequest &request)
    {
        QJsonObject obj;
        obj.insert("teste", 123);

        auto data = QJsonDocument(obj).toJson();

        QHttpHeaders headers;
        headers.append("Content-Type", "application/json");

        QHttpServerResponse response(data);
        response.setHeaders(headers);

        return response;
    };

    m_httpServer.route("/", QHttpServerRequest::Method::Get, index);
    m_httpServer.route("/settings", QHttpServerRequest::Method::Get, settings);
}

// auto getMethod = [](int id, const QHttpServerRequest &request)
// {
//     qDebug() << request.method() << request.headers();
//     QJsonObject myApiObject;
//     QString strId = QString::number(id);
//     myApiObject.insert(strId, "Testando");
//     return QHttpServerResponse(myApiObject);
// };

// auto getRandom = [](const QHttpServerRequest &request) -> QHttpServerResponse
// {
//     qDebug() << request.method() << request.headers();
//     QJsonObject myApiObject;
//     qint64 random = QRandomGenerator::global()->bounded(255);
//     myApiObject.insert("random", random);
//     return QHttpServerResponse(myApiObject);
// };

// httpServer.route("/", QHttpServerRequest::Method::Get, getIndex);
// httpServer.route("/test/<arg>", QHttpServerRequest::Method::Get, getMethod);
// // httpServer.route("/test/random", QHttpServerRequest::Method::Get,getRandom);

// httpServer.route("/test/random", QHttpServerRequest::Method::Get, [](const QHttpServerRequest &request) {
//     return QtConcurrent::run([&]()
//                              {
//                                  qDebug() << request.method() << request.headers();
//                                  QJsonObject myApiObject;
//                                  qint64 random = QRandomGenerator::global()->bounded(255);
//                                  myApiObject.insert("random", random);
//                                  return QHttpServerResponse(myApiObject);
//                              });
// });
