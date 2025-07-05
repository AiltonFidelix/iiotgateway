#include "commmqttactionlistener.h"

#include <QDebug>
#include <mqtt/token.h>

void
CommMQTTActionListener::on_failure(const mqtt::token &asyncActionToken)
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

void
CommMQTTActionListener::on_success(const mqtt::token &asyncActionToken)
{
    auto type = asyncActionToken.get_type();
#warning // TODO: implement signals
}
