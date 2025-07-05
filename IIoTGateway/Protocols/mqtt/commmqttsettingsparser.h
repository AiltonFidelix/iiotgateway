#ifndef COMMMQTTSETTINGSPARSER_H
#define COMMMQTTSETTINGSPARSER_H

#include <QJsonObject>

class MQTTSettingsParser
{
    QJsonObject m_settings;

public:
    MQTTSettingsParser(QJsonObject settings = QJsonObject{});

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

#endif // COMMMQTTSETTINGSPARSER_H
