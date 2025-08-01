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

    qDebug() << Q_FUNC_INFO << "Listening on port:" << tcpserver->serverPort();

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

        const QString response(QStringLiteral("<h1>IIoTGateway - by Ailton Fidelix</h1>"));
        return QHttpServerResponse(response);
    };

    /// OPTIONS handler
    auto options = [](const QHttpServerRequest &request)
    {
        const QByteArray allowMethod(request.headers().value(QStringLiteral("access-control-request-method")));

        QHttpHeaders headers{};
        headers.append(QStringLiteral("Access-Control-Allow-Origin"), QStringLiteral("*"));
        headers.append(QStringLiteral("Access-Control-Allow-Methods"), allowMethod);
        headers.append(QStringLiteral("Access-Control-Allow-Headers"), QStringLiteral("Content-Type"));

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
        const QString username = jsonObj.value(QStringLiteral("username")).toString();
        const QString password = jsonObj.value(QStringLiteral("password")).toString();
        const auto credentials = m_storage->userCredentials();

        auto getData = [](const QString &status, const QString &message) -> QByteArray
        {
            QJsonObject obj{};
            obj.insert(QStringLiteral("status"), status);
            obj.insert(QStringLiteral("message"), message);
            return QJsonDocument(obj).toJson();
        };

        const bool success = ((username == credentials.first) && (password == credentials.second)) ;

        if (!success)
        {
            qWarning() << "Login failed: Wrong credentials";
        }

        const QByteArray data = success ? getData(QStringLiteral("ok"), QStringLiteral("Login success!"))
                                        : getData(QStringLiteral("error"), QStringLiteral("Wrong credentials!"));

        QHttpHeaders headers{};
        headers.append(QStringLiteral("Access-Control-Allow-Origin"), QStringLiteral("*"));
        headers.append(QStringLiteral("Content-Type"), QStringLiteral("application/json"));

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
        obj.insert(QStringLiteral("status"), m_gateway->isRunning() ? QStringLiteral("running") : QStringLiteral("stopped"));

        const QByteArray data = QJsonDocument(obj).toJson(QJsonDocument::Compact);

        QHttpHeaders headers{};
        headers.append(QStringLiteral("Access-Control-Allow-Origin"), QStringLiteral("*"));
        headers.append(QStringLiteral("Content-Type"), QStringLiteral("application/json"));

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
        const QString command = obj.value(QStringLiteral("command")).toString().toLower();

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
        headers.append(QStringLiteral("Access-Control-Allow-Origin"), QStringLiteral("*"));
        headers.append(QStringLiteral("Content-Type"), QStringLiteral("application/json"));

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
        headers.append(QStringLiteral("Access-Control-Allow-Origin"), QStringLiteral("*"));
        headers.append(QStringLiteral("Content-Type"), QStringLiteral("application/json"));

        QHttpServerResponse response(data);
        response.setHeaders(headers);

        return response;
    };

    /// /iiotgateway/settings POST handler
    auto settingsPost = [this](const QHttpServerRequest &request)
    {
        QJsonParseError parser{};

        const QByteArray body = request.body();
        const auto json = QJsonDocument::fromJson(body, &parser);

        if ((parser.error != QJsonParseError::NoError) || (m_storage == nullptr))
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        bool ok = true;

        const QJsonObject settingsObj = json.object();

        qInfo() << Q_FUNC_INFO << "New settings received:" << settingsObj;

        const QJsonObject cloudObj = settingsObj.value(QStringLiteral("cloud")).toObject();
        const QString cloudProtocol = cloudObj.value(QStringLiteral("protocol")).toString();

        ok &= m_storage->setCloudProtocol(cloudProtocol);
        ok &= m_storage->setProtocolSettings(cloudProtocol, cloudObj.value(QStringLiteral("settings")).toObject());

        const QJsonObject edgeObj = settingsObj.value(QStringLiteral("edge")).toObject();
        const QString edgeProtocol = edgeObj.value(QStringLiteral("protocol")).toString();

        ok &= m_storage->setEdgeProtocol(edgeProtocol);
        ok &= m_storage->setProtocolSettings(edgeProtocol, edgeObj.value(QStringLiteral("settings")).toObject());

        if (!ok)
        {
            QHttpServerResponse response(QHttpServerResponse::StatusCode::InternalServerError);
            return response;
        }

        QJsonObject resObj{};
        resObj.insert(QStringLiteral("status"), QStringLiteral("ok"));
        resObj.insert(QStringLiteral("message"), QStringLiteral("Settings successfully saved!"));

        const auto data = QJsonDocument(resObj).toJson(QJsonDocument::Compact);

        QHttpHeaders headers{};
        headers.append(QStringLiteral("Access-Control-Allow-Origin"), QStringLiteral("*"));
        headers.append(QStringLiteral("Content-Type"), QStringLiteral("application/json"));

        QHttpServerResponse response(data);
        response.setHeaders(headers);

        return response;
    };

    m_httpServer.route(QStringLiteral("/"), QHttpServerRequest::Method::Get, index);
    m_httpServer.route(QStringLiteral("/iiotgateway/login"), QHttpServerRequest::Method::Options, options);
    m_httpServer.route(QStringLiteral("/iiotgateway/login"), QHttpServerRequest::Method::Post, loginPost);
    m_httpServer.route(QStringLiteral("/iiotgateway/status"), QHttpServerRequest::Method::Options, options);
    m_httpServer.route(QStringLiteral("/iiotgateway/status"), QHttpServerRequest::Method::Get, statusGet);
    m_httpServer.route(QStringLiteral("/iiotgateway/command"), QHttpServerRequest::Method::Options, options);
    m_httpServer.route(QStringLiteral("/iiotgateway/command"), QHttpServerRequest::Method::Post, commandPost);
    m_httpServer.route(QStringLiteral("/iiotgateway/settings"), QHttpServerRequest::Method::Options, options);
    m_httpServer.route(QStringLiteral("/iiotgateway/settings"), QHttpServerRequest::Method::Get, settingsGet);
    m_httpServer.route(QStringLiteral("/iiotgateway/settings"), QHttpServerRequest::Method::Post, settingsPost);
}
