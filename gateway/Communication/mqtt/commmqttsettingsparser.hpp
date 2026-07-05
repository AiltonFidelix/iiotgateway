#ifndef COMMMQTTSETTINGSPARSER_H
#define COMMMQTTSETTINGSPARSER_H

#include <QJsonObject>

#include "commmqtt_global.hpp"

COMM_MQTT_BEGIN_NAMESPACE

class CommMQTTSettingsParser
{
public:
    explicit CommMQTTSettingsParser(const QJsonObject &settings = QJsonObject());

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

private:
    QJsonObject m_settings;
};

COMM_MQTT_END_NAMESPACE

#endif // COMMMQTTSETTINGSPARSER_H
