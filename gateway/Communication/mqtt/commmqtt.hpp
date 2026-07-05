#ifndef COMMMQTT_H
#define COMMMQTT_H

#include "comminterface.hpp"
#include "commmqtt_global.hpp"

#include "commmqttactionlistener.hpp"
#include "commmqttcallback.hpp"
#include "commmqttsettingsparser.hpp"

#include <QJsonObject>
#include <QThread>
#include <QTimer>

#include <mqtt/async_client.h>

COMM_MQTT_BEGIN_NAMESPACE

class CommMQTT final : public CommInterface
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit CommMQTT(QJsonObject settings = QJsonObject());
    ~CommMQTT();

    bool isconnected() const override;
    void setMQTTClient(mqtt::iasync_client *client);

public slots:
    void connectComm() override;
    void disconnectComm() override;
    void incoming(QByteArray data) override;

private slots:
    void onConnected();
    void onMessageArrived(QByteArray message);

private:
    static int m_typeId;

    int m_pubQos;
    int m_subQos;
    int m_retries;

    bool m_publish;
    bool m_subscribe;

    mqtt::iasync_client *m_client;
    std::string m_pubTopic;
    std::string m_subTopic;

    CommMQTTCallback m_callback;
    CommMQTTActionListener m_listener;
    CommMQTTSettingsParser m_settingsParser;
};

COMM_MQTT_END_NAMESPACE

#endif // COMMMQTT_H
