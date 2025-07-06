#ifndef COMMMQTTSETTINGSPARSER_H
#define COMMMQTTSETTINGSPARSER_H

#include <QJsonObject>

#include "commmqtt_global.h"

COMM_MQTT_BEGIN_NAMESPACE

class MQTTSettingsParser
{
    QJsonObject m_settings;

public:
    explicit MQTTSettingsParser(QJsonObject settings = QJsonObject());
    ~MQTTSettingsParser() = default;

    bool autoReconnect() const;
    bool cleanStart() const;
    bool publish() const;
    bool subscribe() const;

    QString protocol() const;
    QString host() const;
    QString username() const;
    QString password() const;
    QString clientId() const;
    QString publishTopic() const;
    QString subscribeTopic() const;

    int connectionTimeout() const;
    int keepAlive() const;
    int port() const;
    int version() const;
    int publishQos() const;
    int subscribeQos() const;
};

COMM_MQTT_END_NAMESPACE

#endif // COMMMQTTSETTINGSPARSER_H
