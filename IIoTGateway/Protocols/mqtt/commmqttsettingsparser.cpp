#include "commmqttsettingsparser.h"

#include <MQTTAsync.h>

COMM_MQTT_BEGIN_NAMESPACE

MQTTSettingsParser::MQTTSettingsParser(const QJsonObject &settings)
    : m_settings(settings)
{
}

MQTTSettingsParser::MQTTSettingsParser(const QJsonDocument &settings)
    : MQTTSettingsParser(settings.object())
{
}

bool MQTTSettingsParser::autoReconnect() const
{
    return m_settings.value("autoReconnect").toBool(false);
}

bool MQTTSettingsParser::cleanStart() const
{
    return m_settings.value("cleanStart").toBool(false);
}

bool MQTTSettingsParser::publish() const
{
    return m_settings.value("publish").toBool(false);
}

bool MQTTSettingsParser::subscribe() const
{
    return m_settings.value("subscribe").toBool(false);
}

std::string MQTTSettingsParser::protocol() const
{
    const QString protocol = m_settings.value("protocol").toString();
    return protocol.split(':').first().toStdString();
}

std::string MQTTSettingsParser::host() const
{
    return m_settings.value("host").toString().toStdString();
}

std::string MQTTSettingsParser::username() const
{
    return m_settings.value("username").toString().toStdString();
}

std::string MQTTSettingsParser::password() const
{
    return m_settings.value("password").toString().toStdString();
}

std::string MQTTSettingsParser::clientId() const
{
    return m_settings.value("clientId").toString().toStdString();
}

std::string MQTTSettingsParser::publishTopic() const
{
    return m_settings.value("publishTopic").toString().toStdString();
}

std::string MQTTSettingsParser::subscribeTopic() const
{
    return m_settings.value("subscribeTopic").toString().toStdString();
}

int MQTTSettingsParser::connectionTimeout() const
{
    return m_settings.value("connectionTimeout").toInt(10);
}

int
MQTTSettingsParser::keepAlive() const
{
    return m_settings.value("keepAlive").toInt(60);
}

int MQTTSettingsParser::port() const
{
    return m_settings.value("port").toInt(-1);
}

int MQTTSettingsParser::version() const
{
    const QString version = m_settings.value("version").toString();

    if (version == "5.0")
    {
        return MQTTVERSION_5;
    }
    else if (version == "3.1.1")
    {
        return MQTTVERSION_3_1_1;
    }
    else if (version == "3.1")
    {
        return MQTTVERSION_3_1;
    }

    return MQTTVERSION_DEFAULT;
}

int MQTTSettingsParser::publishQos() const
{
    return m_settings.value("publishQos").toInt(0);
}

int MQTTSettingsParser::subscribeQos() const
{
    return m_settings.value("subscribeQos").toInt(0);
}

COMM_MQTT_END_NAMESPACE
