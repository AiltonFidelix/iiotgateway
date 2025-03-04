#include "commmqttcallback.h"

#include <QDebug>

void
CommMQTTCallback::connected(const mqtt::string &message)
{
    qDebug() << "Connected:" << message;
    emit cbConnected();
}

void
CommMQTTCallback::connection_lost(const std::string &cause)
{
    Q_UNUSED(cause)
    qWarning() << "Connection lost!";
    emit cbConnectionLost();
}

void
CommMQTTCallback::message_arrived(mqtt::const_message_ptr ptr)
{
    auto message = QByteArray::fromStdString(ptr->to_string());
    emit cbMessageArrived(message);
}

void
CommMQTTCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    Q_UNUSED(token)
    emit cbDeliveryComplete();
}
