#include "commmqttactionlistener.hpp"

#include <mqtt/token.h>

COMM_MQTT_BEGIN_NAMESPACE

void CommMQTTActionListener::on_success(const mqtt::token &asyncActionToken)
{
    const auto type = asyncActionToken.get_type();

    switch (type)
    {
        case mqtt::token::CONNECT:
            emit connected();
            break;
        case mqtt::token::SUBSCRIBE:
            emit subscribed();
            break;
        case mqtt::token::PUBLISH:
            emit published();
            break;
        case mqtt::token::UNSUBSCRIBE:
            emit unsubscribed();
            break;
        case mqtt::token::DISCONNECT:
            emit disconnected();
            break;
        default:
            break;
    }
}

void CommMQTTActionListener::on_failure(const mqtt::token &asyncActionToken)
{
    const auto type = asyncActionToken.get_type();

    switch (type)
    {
        case mqtt::token::CONNECT:
            emit connectionFailed();
            break;
        case mqtt::token::SUBSCRIBE:
            emit subscribeFailed();
            break;
        case mqtt::token::PUBLISH:
            emit publishFailed();
            break;
        case mqtt::token::UNSUBSCRIBE:
            emit unsubscribeFailed();
            break;
        case mqtt::token::DISCONNECT:
            emit disconnectFailed();
            break;
        default:
            break;
    }
}

COMM_MQTT_END_NAMESPACE
