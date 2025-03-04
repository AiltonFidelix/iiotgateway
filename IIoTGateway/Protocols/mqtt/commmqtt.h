#ifndef COMMMQTT_H
#define COMMMQTT_H

#include "comminterface.h"
#include "commmqttcallback.h"
#include "commmqttactionlistener.h"

#include <QThread>
#include <QTimer>
#include <mqtt/async_client.h>

class CommMQTT : public CommInterface
{
    using ConnOptions = struct {
        std::string username;
        std::string password;
        int mqttVersion;
        int connectTimeout;
        int keepAliveInterval;
        bool reconnect;
        bool cleanSession;
    };

    Q_OBJECT
public:
    Q_INVOKABLE CommMQTT();
    ~CommMQTT();

    bool isconnected() override;

public slots:
    void connectComm() override;
    void disconnectComm() override;
    void incoming(QByteArray data) override;

private slots:
    void onConnected();
    void onMessageArrived(QByteArray message);

private:
    static int m_typeId;
    int m_qos;
    int m_retries;
    bool m_subscribe;

    mqtt::async_client *m_client;
    std::string m_pubTopic;
    std::string m_subTopic;

    ConnOptions m_connOptions;
    CommMQTTCallback m_callback;
    CommMQTTActionListener m_listener;
};

#endif // COMMMQTT_H
