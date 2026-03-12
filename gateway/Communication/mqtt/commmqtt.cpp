#include "commmqtt.h"
#include "commfactory.h"

#include <QDebug>
#include <QJsonDocument>

constexpr const int defaultQos = 0;
constexpr const int defaultConnectionTimeout = 10;
constexpr const int defaultKeepAliveInterval = 60;
constexpr const int maxRetries = 5;

COMM_MQTT_BEGIN_NAMESPACE

int CommMQTT::m_typeId = comm::CommFactory::registerInterface<CommMQTT*>("MQTT");

CommMQTT::CommMQTT(QJsonObject settings) :
    m_pubQos(defaultQos),
    m_subQos(defaultQos),
    m_retries(0),
    m_publish(false),
    m_subscribe(false),
    m_client(nullptr),
    m_pubTopic(""),
    m_subTopic(""),
    m_settingsParser(std::move(settings))
{
    m_publish = m_settingsParser.publish();
    m_subscribe = m_settingsParser.subscribe();
    m_pubTopic = m_settingsParser.publishTopic();
    m_subTopic = m_settingsParser.subscribeTopic();
    m_pubQos = m_settingsParser.publishQos();
    m_subQos = m_settingsParser.subscribeQos();

    connect(&m_callback, &CommMQTTCallback::cbConnected, this, &CommMQTT::onConnected);
    connect(&m_callback, &CommMQTTCallback::cbMessageArrived, this, &CommMQTT::onMessageArrived);
}

CommMQTT::~CommMQTT()
{
    if (m_client)
    {
        delete m_client;
    }
}

bool CommMQTT::isconnected() const
{
    return m_client->is_connected();
}

void CommMQTT::setMQTTClient(mqtt::iasync_client *client)
{
    m_client = client;
}

void CommMQTT::connectComm()
{
    if (m_client == nullptr)
    {
        const std::string protocol = m_settingsParser.protocol();
        const std::string  host = m_settingsParser.host();
        const int port = m_settingsParser.port();

        const std::string server_address = QString("%1://%2%3%4").arg(QString::fromStdString(protocol),
                                                                      QString::fromStdString(host),
                                                                      (port == -1) ? QString() : QString(":"),
                                                                      QString::number(port)).toStdString();

        const std::string client_id = m_settingsParser.clientId();

        m_client = new mqtt::async_client(server_address, client_id);

        m_client->set_callback(m_callback);
    }

    if (m_retries++ >= maxRetries)
    {
        emit connectionFailed();
        return;
    }

    mqtt::connect_options connOpts{};

    const std::string username = m_settingsParser.username();
    const std::string password = m_settingsParser.password();

    if (!username.empty() && !password.empty())
    {
        connOpts.set_user_name(username);
        connOpts.set_password(password);
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

        if (connTok)
        {
            connTok->wait();
            qDebug() << "Successfully connected";
        }
    }
    catch (const mqtt::exception& ex)
    {
        emit error(QString("MQTT connection exception: %1").arg(ex.what()).toUtf8());
    }
}

void CommMQTT::disconnectComm()
{
    if (isconnected())
    {
        auto discTok = m_client->disconnect();

        if (discTok)
        {
            discTok->wait();
        }
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

        if (delTok)
        {
            delTok->wait();
        }
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
    emit outgoing(std::move(message));
}

COMM_MQTT_END_NAMESPACE
