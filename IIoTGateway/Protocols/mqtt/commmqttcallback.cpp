#include "commmqttcallback.h"

#include <QDebug>

void
CommMQTTCallback::connected(const mqtt::string &message)
{
    qDebug() << "Connected: " << message;
}

void
CommMQTTCallback::connection_lost(const std::string &cause)
{
    qDebug() << "Connection lost: " << cause;
}

void
CommMQTTCallback::message_arrived(mqtt::const_message_ptr ptr)
{
    qDebug() << "Message arrived: " << ptr->to_string();
}

void
CommMQTTCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    qDebug()  << "Message delivered" << token->get_message()->to_string();
}
