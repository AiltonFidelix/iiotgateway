#include "commmqtt.h"
#include "commfactory.h"

#include <QDebug>
#include <QJsonDocument>

constexpr int defaultQos = 0;
constexpr int defaultConnectionTimeout = 10;
constexpr int defaultKeepAliveInterval = 60;
constexpr int maxRetries = 5;

COMM_MQTT_BEGIN_NAMESPACE

int CommMQTT::m_typeId = comm::CommFactory::registerInterface<CommMQTT*>("MQTT");

CommMQTT::CommMQTT(QJsonObject settings) :
    m_pubQos{defaultQos},
    m_subQos{defaultQos},
    m_retries{0},
    m_publish{false},
    m_subscribe{false},
    m_client{nullptr},
    m_pubTopic{},
    m_subTopic{},
    m_settingsParser{settings}
{
    const auto protocol = m_settingsParser.protocol();
    const auto host = m_settingsParser.host();
    const auto port = m_settingsParser.port();

    const auto server_address = QString("%1://%2%3%4").arg(protocol, host, (port == -1) ? "" : ":", QString::number(port)).toStdString();
    const auto client_id = m_settingsParser.clientId().toStdString();

    m_publish = m_settingsParser.publish();
    m_subscribe = m_settingsParser.subscribe();
    m_pubTopic = m_settingsParser.publishTopic().toStdString();
    m_subTopic = m_settingsParser.subscribeTopic().toStdString();
    m_pubQos = m_settingsParser.publishQos();
    m_subQos = m_settingsParser.subscribeQos();

    m_client = new mqtt::async_client{server_address, client_id};

    if (m_client == nullptr)
        return;

    connect(&m_callback, &CommMQTTCallback::cbConnected, this, &CommMQTT::onConnected);
    connect(&m_callback, &CommMQTTCallback::cbMessageArrived, this, &CommMQTT::onMessageArrived);

    m_client->set_callback(m_callback);
}

CommMQTT::~CommMQTT()
{
    if (m_client)
    {
        delete m_client;
    }
}

bool
CommMQTT::isconnected()
{
    return m_client->is_connected();
}

void
CommMQTT::connectComm()
{
    if (m_retries++ >= maxRetries)
    {
        emit connectionFailed();
        return;
    }

    mqtt::connect_options connOpts{};

    const auto username = m_settingsParser.username();
    const auto password = m_settingsParser.password();

    if (!username.isEmpty() && !password.isEmpty())
    {
        connOpts.set_user_name(username.toStdString());
        connOpts.set_password(password.toStdString());
    }

    connOpts.set_mqtt_version(m_settingsParser.version());
    connOpts.set_connect_timeout(m_settingsParser.connectionTimeout());
    connOpts.set_keep_alive_interval(m_settingsParser.keepAlive());

    connOpts.set_automatic_reconnect(m_settingsParser.autoReconnect());
    connOpts.set_clean_session(m_settingsParser.cleanStart());

    connect(&m_listener, &CommMQTTActionListener::connectionFailed, this, &CommMQTT::connectComm);

    try
    {
        qDebug() << "Connecting...";
        auto connTok = m_client->connect(connOpts, nullptr, m_listener);
        qDebug() << "Waiting for the connection...";
        connTok->wait();
        qDebug() << "Successfully connected";
    }
    catch (const mqtt::exception& ex)
    {
        emit error(QString("MQTT connection exception: %1").arg(ex.what()).toUtf8());
    }
}

void
CommMQTT::disconnectComm()
{
    if (isconnected())
    {
        auto discTok = m_client->disconnect();
        discTok->wait();
    }

    emit disconnected();
}

void
CommMQTT::incoming(QByteArray data)
{
    if (!isconnected())
    {
        qWarning() << "MQTT publish failed: Not connected!";
        return;
    }

    if (!m_publish)
    {
        qWarning() << "MQTT publish failed: Publish is not configured!";
        return;
    }

    const auto message = data.toStdString();

    try
    {
        auto pubMsg = mqtt::make_message(m_pubTopic, message, m_pubQos, false);
        auto delTok = m_client->publish(pubMsg);
        delTok->wait();
    }
    catch (const mqtt::exception& ex)
    {
        emit error(QString("MQTT publish exception: %1").arg(ex.what()).toUtf8());
    }
}

void
CommMQTT::onConnected()
{
    if (m_subscribe)
    {
        m_client->subscribe(m_subTopic, m_subQos);
        qDebug() << "MQTT topic subscribed:" << m_subTopic;
    }
}

void
CommMQTT::onMessageArrived(QByteArray message)
{
    emit outgoing(message);
}

COMM_MQTT_END_NAMESPACE
