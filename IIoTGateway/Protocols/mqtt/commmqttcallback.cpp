#include "commmqttcallback.h"

#include <QDebug>

void
CommMQTTCallback::connected(const mqtt::string &message)
{
    emit cbConnected(QByteArray::fromStdString(message));
}

void
CommMQTTCallback::connection_lost(const std::string &cause)
{
    emit cbConnection_lost(QByteArray::fromStdString(cause));
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
