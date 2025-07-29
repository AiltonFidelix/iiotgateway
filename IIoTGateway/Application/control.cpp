#include "control.h"
#include "gateway.h"
#include "storageinterface.h"

#include <QTcpServer>
#include <QJsonDocument>
#include <QJsonObject>

Control::Control(QObject *parent)
    : QObject(parent),
    m_httpServer(),
    m_gateway(nullptr),
    m_storage(nullptr)
{
}

Control::~Control()
{
    auto servers = m_httpServer.servers();
    qDeleteAll(servers);
}

bool Control::start(int port)
{
    registerRoutes();

    auto tcpserver = new QTcpServer();

    if (!tcpserver->listen(QHostAddress::Any, port) || !m_httpServer.bind(tcpserver))
    {
        delete tcpserver;
        return false;
    }

    qDebug() << "Listening on port" << tcpserver->serverPort();

    if ((m_gateway != nullptr) && (m_storage != nullptr))
    {
        if (m_storage->active())
            m_gateway->start();
    }

    return true;
}

void Control::stop()
{
    auto servers = m_httpServer.servers();

    for (auto server : std::as_const(servers))
    {
        server->close();
    }
}

void Control::setGateway(Gateway *gateway)
{
    m_gateway = gateway;
}

void Control::setStorage(StorageInterface *storage)
{
    m_storage = storage;
}

void Control::registerRoutes()
{
    /// / index GET handler
    auto index = [](const QHttpServerRequest &request)
    {
        Q_UNUSED(request)

        const QString response("<h1>IIoTGateway - by Ailton Fidelix</h1>");
        return QHttpServerResponse(response);
    };

    /// OPTIONS handler
    auto options = [](const QHttpServerRequest &request)
    {
        const QByteArray allowMethod(request.headers().value("access-control-request-method"));

        QHttpHeaders headers{};
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Access-Control-Allow-Methods", allowMethod);
        headers.append("Access-Control-Allow-Headers", "Content-Type");

        QHttpServerResponse response(QHttpServerResponse::StatusCode::Ok);
        response.setHeaders(headers);

        return response;
    };

    /// /iiotgateway/login POST handler
    auto loginPost = [this](const QHttpServerRequest &request)
    {
        const QByteArray body = request.body();

        QJsonParseError parser{};
        const auto json = QJsonDocument::fromJson(body, &parser);

        if ((parser.error != QJsonParseError::NoError) || (m_storage == nullptr))
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        const QJsonObject jsonObj = json.object();
        const QString username = jsonObj.value("username").toString();
        const QString password = jsonObj.value("password").toString();
        const auto credentials = m_storage->userCredentials();

        auto getData = [](const QString &status, const QString &message) -> QByteArray
        {
            QJsonObject obj{};
            obj.insert("status", status);
            obj.insert("message", message);
            return QJsonDocument(obj).toJson();
        };

        const bool success = ((username == credentials.first) && (password == credentials.second)) ;

        if (!success)
        {
            qWarning() << "Login failed: Wrong credentials";
        }

        const QByteArray data = success ? getData("ok", "Login success!") : getData("error", "Wrong credentials!");

        QHttpHeaders headers{};
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Content-Type", "application/json");

        QHttpServerResponse response(data);
        response.setHeaders(headers);

        return response;
    };

    /// /iiotgateway/status GET handler
    auto statusGet = [this](const QHttpServerRequest &request)
    {
        Q_UNUSED(request)

        if (m_gateway == nullptr)
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        QJsonObject obj{};
        obj.insert("status", m_gateway->isRunning() ? "running" : "stopped");

        const QByteArray data = QJsonDocument(obj).toJson(QJsonDocument::Compact);

        QHttpHeaders headers{};
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Content-Type", "application/json");

        QHttpServerResponse response(data);
        response.setHeaders(headers);

        return response;
    };

    /// /iiotgateway/command POST handler
    auto commandPost = [this](const QHttpServerRequest &request)
    {
        if (m_gateway == nullptr)
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        const QByteArray body = request.body();

        QJsonParseError parser{};
        const auto json = QJsonDocument::fromJson(body, &parser);

        if (parser.error != QJsonParseError::NoError)
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        const QJsonObject obj = json.object();
        const QString command = obj.value("command").toString().toLower();

        if (command == "start")
        {
            m_gateway->start();
        }
        else if (command == "stop")
        {
            m_gateway->stop();
        }
        else if (command == "restart")
        {
            m_gateway->restart();
        }

        QHttpHeaders headers{};
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Content-Type", "application/json");

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

        QJsonObject obj{};

        for (const auto &item : queryItems)
        {
            if (item.first.toLower() != "protocol")
                continue;

            const QString protocol = item.second.toUpper();
            const QJsonObject settings = m_storage->protocolSettings(protocol);

            obj.insert(protocol, settings);
        }

        const QByteArray data = QJsonDocument(obj).toJson(QJsonDocument::Compact);

        QHttpHeaders headers{};
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Content-Type", "application/json");

        QHttpServerResponse response(data);
        response.setHeaders(headers);

        return response;
    };

    /// /iiotgateway/settings POST handler
    auto settingsPost = [this](const QHttpServerRequest &request)
    {
        qInfo() << "New settings received";

        QJsonParseError parser{};

        const QByteArray body = request.body();
        const auto json = QJsonDocument::fromJson(body, &parser);

        if ((parser.error != QJsonParseError::NoError) || (m_storage == nullptr))
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        const QJsonObject reqObj = json.object();
        const QStringList protocols = reqObj.keys();

        bool ok = true;

        for (const auto &protocol : protocols)
        {
            const QJsonValue value = reqObj.value(protocol);

            if (!value.isObject())
                continue;

            const QJsonObject settings = value.toObject();
            ok &= m_storage->setProtocolSettings(protocol, settings);
        }

        if (!ok)
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        QJsonObject resObj{};
        resObj.insert("status", "ok");
        resObj.insert("message", "Settings successfully saved!");

        const auto data = QJsonDocument(resObj).toJson(QJsonDocument::Compact);

        QHttpHeaders headers{};
        headers.append("Access-Control-Allow-Origin", "*");
        headers.append("Content-Type", "application/json");

        QHttpServerResponse response(data);
        response.setHeaders(headers);

        return response;
    };

    m_httpServer.route("/", QHttpServerRequest::Method::Get, index);
    m_httpServer.route("/iiotgateway/login", QHttpServerRequest::Method::Options, options);
    m_httpServer.route("/iiotgateway/login", QHttpServerRequest::Method::Post, loginPost);
    m_httpServer.route("/iiotgateway/status", QHttpServerRequest::Method::Options, options);
    m_httpServer.route("/iiotgateway/status", QHttpServerRequest::Method::Get, statusGet);
    m_httpServer.route("/iiotgateway/command", QHttpServerRequest::Method::Options, options);
    m_httpServer.route("/iiotgateway/command", QHttpServerRequest::Method::Post, commandPost);
    m_httpServer.route("/iiotgateway/settings", QHttpServerRequest::Method::Options, options);
    m_httpServer.route("/iiotgateway/settings", QHttpServerRequest::Method::Get, settingsGet);
    m_httpServer.route("/iiotgateway/settings", QHttpServerRequest::Method::Post, settingsPost);
}
