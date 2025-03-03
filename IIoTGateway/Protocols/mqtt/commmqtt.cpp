#include "commmqtt.h"
#include "commfactory.h"
#include "commmqttactionlistener.h"

#include <QDebug>
#include <QJsonDocument>

constexpr int defaultQos = 0;
constexpr int defaultConnectionTimeout = 10;
constexpr int defaultKeepAliveInterval = 60;
constexpr int maxRetries = 5;

int CommMQTT::m_typeId = CommFactory::registerInterface<CommMQTT*>("MQTT");

CommMQTT::CommMQTT()
    : m_qos(defaultQos),
    m_retries(0),
    m_subscribe(false),
    m_client(nullptr),
    m_pubTopic(std::string()),
    m_subTopic(std::string()),
    m_connOptions(ConnOptions())
{
    const auto protocol = qgetenv("MQTT_PROTOCOL");
    const auto host = qgetenv("MQTT_HOST");

    auto ok = false;
    auto port = qEnvironmentVariableIntValue("MQTT_PORT", &ok);

    const auto server_address = QString("%1://%2%3%4").arg(protocol, host, ok ? ":" : "", QString::number(port)).toStdString();
    const auto client_id = qgetenv("MQTT_CLIENT_ID").toStdString();

    m_pubTopic = qgetenv("MQTT_PUB_TOPIC").toStdString();
    m_subTopic = qgetenv("MQTT_SUB_TOPIC").toStdString();

    m_subscribe = qgetenv("MQTT_SUBSCRIBE").toLower() == "enabled";

    const auto qos = qEnvironmentVariableIntValue("MQTT_QOS", &ok);
    m_qos = ok ? qos : defaultQos;

    m_connOptions.username = qgetenv("MQTT_USERNAME").toStdString();
    m_connOptions.password = qgetenv("MQTT_PASSWORD").toStdString();

    const auto version = qEnvironmentVariableIntValue("MQTT_VERSION", &ok);
    m_connOptions.mqttVersion = ok ? version : MQTTVERSION_DEFAULT;

    const auto timeout = qEnvironmentVariableIntValue("MQTT_CONNECT_TIMEOUT", &ok);
    m_connOptions.connectTimeout = ok ? timeout : defaultConnectionTimeout;

    const auto keepAlive = qEnvironmentVariableIntValue("MQTT_KEEP_ALIVE_INTERVAL", &ok);
    m_connOptions.keepAliveInterval = ok ? keepAlive : defaultKeepAliveInterval;

    m_connOptions.reconnect = qgetenv("MQTT_AUTOMATIC_RECONNECT").toLower() == "enabled";
    m_connOptions.cleanSession = qgetenv("MQTT_CLEAN_SESSION").toLower() == "enabled";

    m_client = new mqtt::async_client(server_address, client_id);

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

    mqtt::connect_options connOpts;

    if (!m_connOptions.username.empty() && !m_connOptions.password.empty())
    {
        connOpts.set_user_name(m_connOptions.username);
        connOpts.set_password(m_connOptions.password);
    }

    connOpts.set_mqtt_version(m_connOptions.mqttVersion);
    connOpts.set_connect_timeout(m_connOptions.connectTimeout);
    connOpts.set_keep_alive_interval(m_connOptions.keepAliveInterval);

    connOpts.set_automatic_reconnect(m_connOptions.reconnect);
    connOpts.set_clean_session(m_connOptions.cleanSession);

    CommMQTTActionListener listener;
    connect(&listener, &CommMQTTActionListener::connectionFailed, this, &CommMQTT::connectComm);

    try
    {
        qDebug() << "Connecting...";
        auto connTok = m_client->connect(connOpts, nullptr, listener);
        qDebug() << "Waiting for the connection...";
        connTok->wait();
    }
    catch (const mqtt::exception& ex)
    {
        emit error(QString("MQTT Exception: %1").arg(ex.what()).toUtf8());
    }

    listener.disconnect();
}

void
CommMQTT::disconnectComm()
{
    if (m_client->is_connected())
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
        return;

    auto message = data.toStdString();

    try
    {
        auto pubMsg = mqtt::make_message(m_pubTopic, message, m_qos, false);
        auto delTok = m_client->publish(pubMsg);
        delTok->wait();
    }
    catch (const mqtt::exception& ex)
    {
        emit error(QString("MQTT Exception: %1").arg(ex.what()).toUtf8());
    }
}

void
CommMQTT::onConnected(QByteArray message)
{
    qDebug() << "Connected!";

    if (m_subscribe)
    {
        qDebug() << "Subscribe:" << m_subTopic;
        m_client->subscribe(m_subTopic, m_qos);
    }
}

void
CommMQTT::onMessageArrived(QByteArray message)
{
    emit outgoing(message);
}
