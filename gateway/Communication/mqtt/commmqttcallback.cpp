#include "commmqttcallback.hpp"

#include <QDebug>

COMM_MQTT_BEGIN_NAMESPACE

void CommMQTTCallback::connected(const mqtt::string &message)
{
    qInfo() << "Connected:" << message;
    emit cbConnected();
}

void CommMQTTCallback::connection_lost(const std::string &cause)
{
    qWarning() << "Connection lost:" << cause;
    emit cbConnectionLost();
}

void CommMQTTCallback::message_arrived(mqtt::const_message_ptr ptr)
{
    auto message = QByteArray::fromStdString(ptr->to_string());

    qDebug() << "Message arrived:" << message;

    emit cbMessageArrived(std::move(message));
}

void CommMQTTCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    Q_UNUSED(token)

    emit cbDeliveryComplete();
}

COMM_MQTT_END_NAMESPACE
