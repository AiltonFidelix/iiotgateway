#include "commmqtt.h"
#include "commfactory.h"

#include <QDebug>
#include <QJsonDocument>

int CommMQTT::m_typeId = CommFactory::registerInterface<CommMQTT*>("MQTT");

bool
CommMQTT::isconnected()
{
    return m_client->is_connected();
}

void
CommMQTT::connectComm()
{
#warning // TODO: read from enviroment vaiables

    const std::string server_address("mqtt://localhost:1883");
    const std::string client_id("gateway");
    m_topic = "gateway/topic";
    m_qos = 1;

    const std::string LWT_PAYLOAD{"Last will and testament."};

    m_client = new mqtt::async_client(server_address, client_id);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(60);
    connOpts.set_clean_session(true);

    try
    {
        qDebug() << "Connecting...";
        m_client->set_callback(m_callback);

        auto connTok = m_client->connect(connOpts);
        qDebug() << "Waiting for the connection...";
        connTok->wait();
    }
    catch (const mqtt::exception& ex)
    {
        emit error(QString("MQTT Exception: %1").arg(ex.what()).toUtf8());
    }
}

void
CommMQTT::disconnectComm()
{
    auto discTok = m_client->disconnect();
    discTok->wait();
}

void
CommMQTT::incoming(QByteArray data)
{
#warning // TODO: handle errors
    auto message = data.toStdString();

    try
    {
        auto pubMsg = mqtt::make_message(m_topic, message, m_qos, false);
        auto delTok = m_client->publish(pubMsg);
        delTok->wait();
    }
    catch (const mqtt::exception& ex)
    {
        emit error(QString("MQTT Exception: %1").arg(ex.what()).toUtf8());
    }
}

// docker exec -it mosquitto mosquitto_sub -h localhost -t gateway/topic
// docker exec -it mosquitto mosquitto_pub -h localhost -t gateway/topic -m "Hello from Docker!"

