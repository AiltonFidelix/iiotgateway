#ifndef COMMMQTT_H
#define COMMMQTT_H

#include <QThread>
#include <QTimer>
#include <QJsonObject>
#include <mqtt/async_client.h>

#include "commmqtt_global.h"
#include "comminterface.h"
#include "commmqttcallback.h"
#include "commmqttactionlistener.h"
#include "commmqttsettingsparser.h"

COMM_MQTT_BEGIN_NAMESPACE

class CommMQTT : public CommInterface
{
    Q_OBJECT

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

public:
    Q_INVOKABLE explicit CommMQTT(const QJsonObject &settings = QJsonObject());
    ~CommMQTT();

    bool isconnected() override;
    void setMQTTClient(mqtt::iasync_client *client);

public slots:
    void connectComm() override;
    void disconnectComm() override;
    void incoming(QByteArray data) override;

private slots:
    void onConnected();
    void onMessageArrived(QByteArray message);
};

COMM_MQTT_END_NAMESPACE

#endif // COMMMQTT_H
