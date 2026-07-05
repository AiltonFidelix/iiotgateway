#ifndef COMMMQTTACTIONLISTENER_H
#define COMMMQTTACTIONLISTENER_H

#include "commmqtt_global.hpp"

#include <QObject>

#include <mqtt/iaction_listener.h>

COMM_MQTT_BEGIN_NAMESPACE

class CommMQTTActionListener final : public QObject, public virtual mqtt::iaction_listener
{
    Q_OBJECT
public:
    void on_success(const mqtt::token &asyncActionToken);
    void on_failure(const mqtt::token &asyncActionToken);

signals:
    void connected();
    void subscribed();
    void published();
    void unsubscribed();
    void disconnected();

    void connectionFailed();
    void subscribeFailed();
    void publishFailed();
    void unsubscribeFailed();
    void disconnectFailed();
};

COMM_MQTT_END_NAMESPACE

#endif // COMMMQTTACTIONLISTENER_H
