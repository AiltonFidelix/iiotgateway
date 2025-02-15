#ifndef COMMMQTTCALLBACK_H
#define COMMMQTTCALLBACK_H

#include <QObject>
#include "mqtt/callback.h"

class CommMQTTCallback : public QObject, public virtual mqtt::callback
{
    Q_OBJECT
public:
    // CommMQTTCallback() = default;
    // ~CommMQTTCallback() = default;

    void connected(const mqtt::string &message) override;
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr ptr) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

signals:
    void cbConnected();
    void cbConnection_lost(const QByteArray &cause);
    void cbMessage_arrived(const QByteArray &message);
    void cbDelivery_complete();
};

#endif // COMMMQTTCALLBACK_H
