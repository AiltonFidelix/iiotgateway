#include "gatewaycontroller.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QNetworkRequest>
#include <QMetaEnum>
#include <QJsonDocument>
#include <QJsonObject>

#ifdef Q_OS_WASM
#include <emscripten/emscripten.h>
#include <emscripten/val.h>

EM_JS(emscripten::EM_VAL, getServerUrl, (), {
    let url = window.location.protocol + "//" + window.location.hostname;
    return Emval.toHandle(url);
});
#endif

GatewayController::GatewayController(QObject *parent)
    : QObject{parent}
{
#ifdef Q_OS_WASM
    auto url = emscripten::val::take_ownership(getServerUrl());
    m_serverUrl = QString("%1:8084").arg(QString::fromStdString(url.as<std::string>()));
#else
    m_serverUrl = "http://localhost:8084";
#endif

    m_manager.setTransferTimeout(5000);
}

void
GatewayController::login(const QString &username, const QString &password)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(password.toUtf8());

    const auto hashPass = QString(hash.result().toHex());

    QJsonObject obj;
    obj.insert("username", username);
    obj.insert("password", hashPass);

    QNetworkRequest request(QUrl(QString("%1/iiotgateway/login").arg(m_serverUrl)));
    request.setRawHeader("Content-Type", "application/json");

    auto reply = m_manager.post(request, QJsonDocument(obj).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError e){
        const QString errorOcurred = errorHandler(e);
        const QString errorMessage = QString("Login failed!\n\n%1").arg(errorOcurred);
        emit error(errorMessage);
    });

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        reply->disconnect();

        auto data = reply->readAll();

        reply->deleteLater();

        QJsonParseError parser;
        auto doc = QJsonDocument::fromJson(data, &parser);

        if (parser.error != QJsonParseError::NoError)
        {
            const QString errorMessage = QString("Login failed!\n\n%1").arg(parser.errorString());
            emit error(errorMessage);
            return;
        }

        auto obj = doc.object();
        auto status = obj.value("status").toString();

        if (status.toLower() == "error")
        {
            const QString errorOcurred = obj.value("message").toString();
            const QString errorMessage = QString("Login failed!\n\n%1").arg(errorOcurred);
            emit error(errorMessage);
            return;
        }

        emit success(QByteArray());
    });
}

void
GatewayController::setSettings(const QByteArray &settings)
{
    QNetworkRequest request(QUrl(QString("%1/iiotgateway/settings").arg(m_serverUrl)));
    request.setRawHeader("Content-Type", "application/json");

    auto reply = m_manager.post(request, settings);

    connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError e){
        const QString errorOcurred = errorHandler(e);
        const QString errorMessage = QString("Failed to save device settings!\n\n%1").arg(errorOcurred);
        emit error(errorMessage);
    });

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        reply->disconnect();
        reply->deleteLater();

        emit success("Successfully saved!");
    });
}

void
GatewayController::requestSettings(const QStringList &protocols)
{
    QString query = !protocols.isEmpty() ? "?" : "";

    for (const auto &protocol : protocols)
    {
        query.append(QString("protocol=%1&").arg(protocol));
    }

    query.removeLast();

    QNetworkRequest request(QUrl(QString("%1/iiotgateway/settings%2").arg(m_serverUrl, query)));
    request.setRawHeader("Content-Type", "application/json");

    auto reply = m_manager.get(request);

    connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError e){
        const QString errorOcurred = errorHandler(e);
        const QString errorMessage = QString("Failed to load device settings!\n\n%1").arg(errorOcurred);
        emit error(errorMessage);
    });

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        reply->disconnect();

        auto data = reply->readAll();
        reply->deleteLater();

        emit settings(data);
    });
}

void
GatewayController::requestStatus()
{
    QNetworkRequest request(QUrl(QString("%1/iiotgateway/status").arg(m_serverUrl)));
    request.setRawHeader("Content-Type", "application/json");

    auto reply = m_manager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        reply->disconnect();

        auto data = reply->readAll();

        reply->deleteLater();

        emit status(data);
    });
}

void
GatewayController::start()
{
    const QString cmd = "start";
    sendCommand(cmd);
}

void
GatewayController::stop()
{
    const QString cmd = "stop";
    sendCommand(cmd);
}

void
GatewayController::restart()
{
    const QString cmd = "restart";
    sendCommand(cmd);
}

void
GatewayController::sendCommand(const QString &command)
{
    QNetworkRequest request(QUrl(QString("%1/iiotgateway/command").arg(m_serverUrl)));
    request.setRawHeader("Content-Type", "application/json");

    QJsonObject obj;
    obj.insert("command", command);

    auto reply = m_manager.post(request, QJsonDocument(obj).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError e){
        const QString errorOcurred = errorHandler(e);
        const QString errorMessage = QString("Failed to execute command!\n\n%1").arg(errorOcurred);
        emit error(errorMessage);
    });

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        reply->disconnect();
        reply->deleteLater();
#warning // TODO: handle response message
    });
}

QString
GatewayController::errorHandler(QNetworkReply::NetworkError e)
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    reply->disconnect();
    reply->deleteLater();

    auto metaEnum = QMetaEnum::fromType<QNetworkReply::NetworkError>();
    auto errorMessage = metaEnum.valueToKey(e);

    return QString("Error occurred: %1").arg(errorMessage);
}

