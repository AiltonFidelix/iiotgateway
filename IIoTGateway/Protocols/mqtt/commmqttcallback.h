#ifndef COMMMQTTCALLBACK_H
#define COMMMQTTCALLBACK_H

#include <QObject>
#include <mqtt/callback.h>

#include "commmqtt_global.h"

COMM_MQTT_BEGIN_NAMESPACE

class CommMQTTCallback : public QObject, public virtual mqtt::callback
{
    Q_OBJECT
public:
    void connected(const mqtt::string &message) override;
    void connection_lost(const std::string &cause) override;
    void message_arrived(mqtt::const_message_ptr ptr) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

signals:
    void cbConnected();
    void cbConnectionLost();
    void cbMessageArrived(QByteArray message);
    void cbDeliveryComplete();
};

COMM_MQTT_END_NAMESPACE

#endif // COMMMQTTCALLBACK_H

