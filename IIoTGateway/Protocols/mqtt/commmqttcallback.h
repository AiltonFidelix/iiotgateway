#ifndef COMMMQTTCALLBACK_H
#define COMMMQTTCALLBACK_H

#include <QObject>
#include "mqtt/callback.h"

class CommMQTTCallback : public virtual mqtt::callback
{
    // Q_OBJECT
public:
    // CommMQTTCallback() = default;
    // ~CommMQTTCallback() = default;

    void connected(const mqtt::string &message) override;
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr ptr) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

// signals:
//     void mqtt_connected();
//     void mqtt_connection_lost();
//     void mqtt_message_arrived(const QString &message);
//     void mqtt_delivery_complete();
};

#endif // COMMMQTTCALLBACK_H
