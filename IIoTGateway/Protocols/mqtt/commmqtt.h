#ifndef COMMMQTT_H
#define COMMMQTT_H

#include "../comminterface.h"
#include "commmqttcallback.h"
#include "mqtt/async_client.h"

#include <QThread>
#include <QTimer>

class CommMQTT : public CommInterface
{
    Q_OBJECT
public:
    Q_INVOKABLE CommMQTT() = default;
    ~CommMQTT() = default;

    bool isconnected() override;

public slots:
    void connectComm() override;
    void disconnectComm() override;
    void incoming(QByteArray data) override;

private:
    static int m_typeId;
    mqtt::async_client *m_client;
    std::string m_topic;
    int m_qos;

    CommMQTTCallback m_callback;
};

#endif // COMMMQTT_H
