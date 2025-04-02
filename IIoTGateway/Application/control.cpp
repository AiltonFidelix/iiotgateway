#include "control.h"
#include "gateway.h"
#include "storageinterface.h"

#include <QTcpServer>
#include <QJsonDocument>
#include <QJsonObject>

Control::Control(QObject *parent)
    : QObject(parent)
{
}

Control::~Control()
{
    auto servers = m_httpServer.servers();
    qDeleteAll(servers);
}

bool
Control::start(int port)
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
Control::stop()
{
    auto servers = m_httpServer.servers();

    for (auto server : std::as_const(servers))
    {
        server->close();
    }
}

void
Control::setGateway(Gateway *gateway)
{
    m_gateway = gateway;
}

void
Control::setStorage(StorageInterface *storage)
{
    m_storage = storage;
}

void
Control::registerRoutes()
{
    /// / index GET handler
    auto index = [](const QHttpServerRequest &request)
    {
        QString response = "<h1>IIoTGateway - by Ailton Fidelix</h1>";
        return QHttpServerResponse(response);
    };

    /// /iiotgateway/login OPTIONS handler
    auto loginOptions = [](const QHttpServerRequest &request)
    {
        QHttpHeaders headers;
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Access-Control-Allow-Methods", "POST");
        headers.append("Access-Control-Allow-Headers", "Content-Type");

        QHttpServerResponse response(QHttpServerResponse::StatusCode::Ok);
        response.setHeaders(headers);

        return response;
    };

    /// /iiotgateway/login POST handler
    auto loginPost = [](const QHttpServerRequest &request)
    {
        const auto body = request.body();

        QJsonParseError parser;
        const auto json = QJsonDocument::fromJson(body, &parser);

        if (parser.error != QJsonParseError::NoError)
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        const auto obj = json.object();

        const QString username = obj.value("username").toString();
        const QString password = obj.value("password").toString();

        QByteArray data;

#warning // TODO: get user and pass from database
        if ((username == "admin") && (password == "admin"))
        {
            QJsonObject obj;
            obj.insert("status", "ok");
            obj.insert("message", "Login success!");
            data = QJsonDocument(obj).toJson();
        }
        else
        {
            QJsonObject obj;
            obj.insert("status", "error");
            obj.insert("message", "Wrong credentials!");
            data = QJsonDocument(obj).toJson();
        }

        QHttpHeaders headers;
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Content-Type", "application/json");

        QHttpServerResponse response(data);
        response.setHeaders(headers);

        return response;
    };

    /// /iiotgateway/status OPTIONS handler
    auto statusOptions = [](const QHttpServerRequest &request)
    {
        QHttpHeaders headers;
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Access-Control-Allow-Methods", "GET");
        headers.append("Access-Control-Allow-Headers", "Content-Type");

        QHttpServerResponse response(QHttpServerResponse::StatusCode::Ok);
        response.setHeaders(headers);

        return response;
    };

    /// /iiotgateway/status GET handler
    auto statusGet = [this](const QHttpServerRequest &request)
    {
        if (m_gateway == nullptr)
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        QJsonObject obj;
        obj.insert("status", m_gateway->isRunning() ? "running" : "stopped");

        const auto data = QJsonDocument(obj).toJson(QJsonDocument::Compact);

        QHttpHeaders headers;
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Content-Type", "application/json");

        QHttpServerResponse response(data);
        response.setHeaders(headers);

        return response;
    };

    /// /iiotgateway/settings OPTIONS handler
    auto settingsOptions = [](const QHttpServerRequest &request)
    {
        QHttpHeaders headers;
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        headers.append("Access-Control-Allow-Headers", "Content-Type");

        QHttpServerResponse response(QHttpServerResponse::StatusCode::Ok);
        response.setHeaders(headers);

        return response;
    };

    /// /iiotgateway/settings GET handler
    auto settingsGet = [this](const QHttpServerRequest &request)
    {
        if (m_storage == nullptr)
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        const auto queryItems = request.query().queryItems();

        QJsonObject obj;

        for (const auto &item : queryItems)
        {
            if (item.first.toLower() != "protocol")
                continue;

            const auto protocol = item.second.toUpper();
            const auto settings = m_storage->protocolSettings(protocol);

            obj.insert(protocol, settings);
        }
        const auto data = QJsonDocument(obj).toJson(QJsonDocument::Compact);

        QHttpHeaders headers;
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Content-Type", "application/json");

        QHttpServerResponse response(data);
        response.setHeaders(headers);

        return response;
    };

    /// /iiotgateway/settings POST handler
    auto settingsPost = [this](const QHttpServerRequest &request)
    {
        qDebug() << "New settings received";

        QJsonParseError parser;

        const auto body = request.body();
        const auto json = QJsonDocument::fromJson(body, &parser);

        if ((parser.error != QJsonParseError::NoError) || (m_storage == nullptr))
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        const auto reqObj = json.object();
        const auto keys = reqObj.keys();

        bool ok = true;

        for (const auto &key : keys)
        {
            auto value = reqObj.value(key);

            if (!value.isObject())
                continue;

            auto settings = value.toObject();
            ok &= m_storage->setProtocolSettings(key, settings);
        }

        if (!ok)
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        QJsonObject resObj;
        resObj.insert("status", "ok");
        resObj.insert("message", "Settings successfully saved!");

        const auto data = QJsonDocument(resObj).toJson(QJsonDocument::Compact);

        QHttpHeaders headers;
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Content-Type", "application/json");

        QHttpServerResponse response(data);
        response.setHeaders(headers);

        return response;
    };

    m_httpServer.route("/", QHttpServerRequest::Method::Get, index);
    m_httpServer.route("/iiotgateway/login", QHttpServerRequest::Method::Options, loginOptions);
    m_httpServer.route("/iiotgateway/login", QHttpServerRequest::Method::Post, loginPost);
    m_httpServer.route("/iiotgateway/status", QHttpServerRequest::Method::Options, statusOptions);
    m_httpServer.route("/iiotgateway/status", QHttpServerRequest::Method::Post, statusGet);
    m_httpServer.route("/iiotgateway/settings", QHttpServerRequest::Method::Options, settingsOptions);
    m_httpServer.route("/iiotgateway/settings", QHttpServerRequest::Method::Get, settingsGet);
    m_httpServer.route("/iiotgateway/settings", QHttpServerRequest::Method::Post, settingsPost);
}
