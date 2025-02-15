#include "commmqttcallback.h"

#include <QDebug>

void
CommMQTTCallback::connected(const mqtt::string &message)
{
    qDebug() << "Connected: " << message;

    emit cbConnected();
}

void
CommMQTTCallback::connection_lost(const std::string &cause)
{
    auto message = QByteArray::fromStdString(cause);

    qDebug() << "Connection lost: " << message;

    emit cbConnection_lost(message);
}

void
CommMQTTCallback::message_arrived(mqtt::const_message_ptr ptr)
{
    auto message = QByteArray::fromStdString(ptr->to_string());

    qDebug() << "Message arrived: " << message;

    emit cbMessage_arrived(message);
}

void
CommMQTTCallback::delivery_complete(mqtt::delivery_token_ptr token)
{
    // qDebug()  << "Message delivered" << token->get_message()->to_string();
    qDebug()  << "Message delivered is complete:" << token->is_complete();
    qDebug()  << "Message delivered code:" << token->get_return_code();

    emit cbDelivery_complete();
}
