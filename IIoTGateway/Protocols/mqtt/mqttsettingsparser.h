#ifndef MQTTSETTINGSPARSER_H
#define MQTTSETTINGSPARSER_H

#include <QJsonObject>

#include "commmqtt_global.h"

COMM_MQTT_BEGIN_NAMESPACE

class MQTTSettingsParser
{
    QJsonObject m_settings;

public:
    explicit MQTTSettingsParser(const QJsonObject &settings = QJsonObject());
    ~MQTTSettingsParser() = default;

    std::string protocol() const;
    std::string host() const;
    std::string username() const;
    std::string password() const;
    std::string clientId() const;
    std::string publishTopic() const;
    std::string subscribeTopic() const;

    int connectionTimeout() const;
    int keepAlive() const;
    int port() const;
    int version() const;
    int publishQos() const;
    int subscribeQos() const;

    bool autoReconnect() const;
    bool cleanStart() const;
    bool publish() const;
    bool subscribe() const;
};

COMM_MQTT_END_NAMESPACE

#endif // MQTTSETTINGSPARSER_H
