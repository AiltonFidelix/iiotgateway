#include "interface.h"

ControlServer::ControlServer(QObject *parent)
    : QObject{parent}
{}


// QHttpServer httpServer;

// const auto port = httpServer.listen(QHostAddress::Any, 8083);

// if (!port)
//     return 0;

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

// auto getIndex = [](const QHttpServerRequest &request)
// {
//     qDebug() << request.method() << request.headers();
//     QString response = "<h1>It worked</h1";
//     return QHttpServerResponse(response);
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


// QHttpServer httpServer;

// const auto port = httpServer.listen(QHostAddress::Any, 8083);

// if (!port)
//     return 0;

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

// auto getIndex = [](const QHttpServerRequest &request)
// {
//     qDebug() << request.method() << request.headers();
//     QString response = "<h1>It worked</h1";
//     return QHttpServerResponse(response);
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

// qDebug() << QCoreApplication::translate("QHttpServerExample",
//                                         "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port);
