#ifndef COMMMQTTACTIONLISTENER_H
#define COMMMQTTACTIONLISTENER_H

#include <QObject>
#include <mqtt/iaction_listener.h>

#include "commmqtt_global.h"

COMM_MQTT_BEGIN_NAMESPACE

class CommMQTTActionListener : public QObject, public virtual mqtt::iaction_listener
{
    Q_OBJECT
public:
    void on_failure(const mqtt::token &asyncActionToken);
    void on_success(const mqtt::token &asyncActionToken);

signals:
    void connectionFailed();
    void subscribeFailed();
    void publishFailed();
    void unsubscribeFailed();
    void disconnectFailed();
};

COMM_MQTT_END_NAMESPACE

#endif // COMMMQTTACTIONLISTENER_H
