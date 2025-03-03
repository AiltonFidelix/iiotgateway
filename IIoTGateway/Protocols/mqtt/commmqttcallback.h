#ifndef COMMMQTTCALLBACK_H
#define COMMMQTTCALLBACK_H

#include <QObject>
#include <mqtt/callback.h>

class CommMQTTCallback : public QObject, public virtual mqtt::callback
{
    Q_OBJECT
public:
    void connected(const mqtt::string &message) override;
    void connection_lost(const std::string& cause) override;
    void message_arrived(mqtt::const_message_ptr ptr) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

signals:
    void cbConnected(QByteArray message);
    void cbConnection_lost(QByteArray cause);
    void cbMessageArrived(QByteArray message);
    void cbDeliveryComplete();
};

#endif // COMMMQTTCALLBACK_H

