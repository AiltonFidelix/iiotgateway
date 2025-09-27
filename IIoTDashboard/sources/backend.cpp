#include "backend.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QNetworkRequest>
#include <QMetaEnum>
#include <QJsonDocument>
#include <QJsonObject>

constexpr const int TIMEOUT = 5000;

#ifdef Q_OS_WASM
#include <emscripten/emscripten.h>
#include <emscripten/val.h>

EM_JS(emscripten::EM_VAL, getServerUrl, (), {
    let url = window.location.protocol + "//" + window.location.hostname;
    return Emval.toHandle(url);
});
#endif

Backend::Backend(QObject *parent)
    : QObject(parent),
    m_manager(),
    m_serverUrl(QStringLiteral("http://localhost:8084"))
{
#ifdef Q_OS_WASM
    auto url = emscripten::val::take_ownership(getServerUrl());
    m_serverUrl = QString("%1:8084").arg(QString::fromStdString(url.as<std::string>()));
#endif

    m_manager.setTransferTimeout(TIMEOUT);
}

void Backend::login(const QString &username, const QString &password)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(password.toUtf8());

    const QString hashPass(hash.result().toHex());

    QJsonObject obj{};
    obj.insert(QStringLiteral("username"), username);
    obj.insert(QStringLiteral("password"), hashPass);

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
        auto resp = parserReply(reply);

        if (resp.first)
        {
            emit success(QByteArray());
        }
        else
        {
            emit error(resp.second);
        }
    });
}

void Backend::setCommunicationSettings(const QByteArray &settings)
{
    QNetworkRequest request(QUrl(QString("%1/iiotgateway/communication").arg(m_serverUrl)));
    request.setRawHeader("Content-Type", "application/json");

    auto reply = m_manager.post(request, settings);

    connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError e){
        const QString errorOcurred = errorHandler(e);
        const QString errorMessage = QString("Failed to save communication settings!\n\n%1").arg(errorOcurred);
        emit error(errorMessage);
    });

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        reply->disconnect();
        reply->deleteLater();

        emit success(QStringLiteral("Successfully saved!"));
    });
}

void Backend::requestCommunicationSettings(const QStringList &protocols)
{
    QString query = !protocols.isEmpty() ? QStringLiteral("?") : "";

    for (const auto &protocol : protocols)
    {
        query.append(QString("protocol=%1&").arg(protocol));
    }

    query.removeLast();

    QNetworkRequest request(QUrl(QString("%1/iiotgateway/communication%2").arg(m_serverUrl, query)));
    request.setRawHeader("Content-Type", "application/json");

    auto reply = m_manager.get(request);

    connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError e){
        const QString errorOcurred = errorHandler(e);
        const QString errorMessage = QString("Failed to load communication settings!\n\n%1").arg(errorOcurred);

        emit error(errorMessage);
    });

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        reply->disconnect();

        auto data = reply->readAll();
        reply->deleteLater();

        emit communicationSettings(data);
    });
}

void Backend::setNetworkSettings(const QByteArray &settings)
{
    QNetworkRequest request(QUrl(QString("%1/iiotgateway/network").arg(m_serverUrl)));
    request.setRawHeader("Content-Type", "application/json");

    auto reply = m_manager.post(request, settings);

    connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError e){
        const QString errorOcurred = errorHandler(e);
        const QString errorMessage = QString("Failed to save network settings!\n\n%1").arg(errorOcurred);
        emit error(errorMessage);
    });

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        reply->disconnect();
        reply->deleteLater();

        emit success(QStringLiteral("Successfully saved!"));
    });
}

void Backend::requestNetworkSettings()
{
    QNetworkRequest request(QUrl(QString("%1/iiotgateway/network").arg(m_serverUrl)));
    request.setRawHeader("Content-Type", "application/json");

    auto reply = m_manager.get(request);

    connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError e){
        const QString errorOcurred = errorHandler(e);
        const QString errorMessage = QString("Failed to load network settings!\n\n%1").arg(errorOcurred);

        emit error(errorMessage);
    });

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        reply->disconnect();

        auto data = reply->readAll();
        reply->deleteLater();

        emit networkSettings(data);
    });
}

void Backend::requestStatus()
{
    QNetworkRequest request(QUrl(QString("%1/iiotgateway/status").arg(m_serverUrl)));
    request.setRawHeader("Content-Type", "application/json");

    auto reply = m_manager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        auto resp = parserReply(reply);

        if (resp.first)
        {
            emit status(resp.second);
        }
        else
        {
            emit error(resp.second);
        }
    });
}

void Backend::start()
{
    sendCommand(QStringLiteral("start"));
}

void Backend::stop()
{
    sendCommand(QStringLiteral("stop"));
}

void Backend::restart()
{
    sendCommand(QStringLiteral("restart"));
}

void Backend::reboot()
{
    sendCommand(QStringLiteral("reboot"));
}

QPair<bool, QString> Backend::parserReply(QNetworkReply *reply) const
{
    if (reply == nullptr)
    {
        return qMakePair(false, QStringLiteral("Server haven't replied!"));
    }

    reply->disconnect();

    const QByteArray data = reply->readAll();

    reply->deleteLater();

    QJsonParseError parser{};
    auto doc = QJsonDocument::fromJson(data, &parser);

    if (parser.error != QJsonParseError::NoError)
    {
        const QString errorMessage = QString("Failed!\n\n%1").arg(parser.errorString());
        return qMakePair(false, errorMessage);
    }

    const QJsonObject obj = doc.object();
    const QString status = obj.value(QStringLiteral("status")).toString();
    const QString message = obj.value(QStringLiteral("message")).toString();

    const bool ok = status == "ok";

    return qMakePair(ok, message);
}

void Backend::sendCommand(const QString &command)
{
    QNetworkRequest request(QUrl(QString("%1/iiotgateway/command").arg(m_serverUrl)));
    request.setRawHeader("Content-Type", "application/json");

    QJsonObject obj{};
    obj.insert(QStringLiteral("command"), command);

    auto reply = m_manager.post(request, QJsonDocument(obj).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError e){
        const QString errorOcurred = errorHandler(e);
        const QString errorMessage = QString("Failed to execute command!\n\n%1").arg(errorOcurred);
        emit error(errorMessage);
    });

    connect(reply, &QNetworkReply::finished, this, [this](){
        auto reply = qobject_cast<QNetworkReply*>(sender());
        auto resp = parserReply(reply);

        if (resp.first)
        {
            emit success(resp.second);
        }
        else
        {
            emit error(resp.second);
        }
    });
}

QString Backend::errorHandler(QNetworkReply::NetworkError e)
{
    auto reply = qobject_cast<QNetworkReply*>(sender());

    if (reply != nullptr)
    {
        reply->disconnect();
        reply->deleteLater();
    }

    auto metaEnum = QMetaEnum::fromType<QNetworkReply::NetworkError>();
    auto errorMessage = metaEnum.valueToKey(e);

    return QString("Error occurred: %1").arg(errorMessage);
}

