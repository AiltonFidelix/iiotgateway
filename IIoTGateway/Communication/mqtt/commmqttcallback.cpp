#include "commmqttcallback.h"

#include <QDebug>

COMM_MQTT_BEGIN_NAMESPACE

void CommMQTTCallback::connected(const mqtt::string &message)
{
    qInfo() << Q_FUNC_INFO << "Connected:" << message;
    emit cbConnected();
}

void CommMQTTCallback::connection_lost(const std::string &cause)
{
    qWarning() << Q_FUNC_INFO << "Cause:" << cause;
    emit cbConnectionLost();
}

void CommMQTTCallback::message_arrived(mqtt::const_message_ptr ptr)
{
    const auto message = QByteArray::fromStdString(ptr->to_string());

    qInfo() << Q_FUNC_INFO << "Message:" << message;

    emit cbMessageArrived(message);
}

void CommMQTTCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    Q_UNUSED(token)

    qInfo() << Q_FUNC_INFO;

    emit cbDeliveryComplete();
}

COMM_MQTT_END_NAMESPACE
