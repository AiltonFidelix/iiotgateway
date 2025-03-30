#include "gatewaycontroller.h"

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
GatewayController::setSettings(const QByteArray &settings)
{
    QHttpHeaders headers;
    headers.append("Content-Type", "application/json");

    QNetworkRequest request((QUrl(m_serverUrl)));
    request.setHeaders(headers);

    auto reply = m_manager.post(request, settings);

    connect(reply, &QNetworkReply::errorOccurred, this, &GatewayController::errorHandler);

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        reply->disconnect();
        reply->deleteLater();

        emit success("Successfully saved!");
    });
}

void
GatewayController::requestSettings()
{
    QHttpHeaders headers;
    headers.append("Content-Type", "application/json");

    QNetworkRequest request((QUrl(m_serverUrl)));
    request.setHeaders(headers);

    auto reply = m_manager.get(request);

    connect(reply, &QNetworkReply::errorOccurred, this, &GatewayController::errorHandler);

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
    QHttpHeaders headers;
    headers.append("Content-Type", "application/json");

    QNetworkRequest request((QUrl(QString("%1/iiotgateway/status").arg(m_serverUrl))));
    request.setHeaders(headers);

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
    QHttpHeaders headers;
    headers.append("Content-Type", "application/json");

    QNetworkRequest request((QUrl(QString("%1/iiotgateway/command").arg(m_serverUrl))));
    request.setHeaders(headers);

    QJsonObject obj;
    obj.insert("command", command);

    auto reply = m_manager.post(request, QJsonDocument(obj).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::errorOccurred, this, &GatewayController::errorHandler);

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        reply->disconnect();
        reply->deleteLater();
    });
}

void
GatewayController::errorHandler(QNetworkReply::NetworkError e)
{
    auto reply = qobject_cast<QNetworkReply*>(sender());
    reply->disconnect();
    reply->deleteLater();

    auto metaEnum = QMetaEnum::fromType<QNetworkReply::NetworkError>();
    auto errorMessage = metaEnum.valueToKey(e);

    emit error(QString("Error occurred: %1").arg(errorMessage).toUtf8());
}

