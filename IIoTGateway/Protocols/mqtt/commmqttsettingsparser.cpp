#include "commmqttsettingsparser.h"

#include <MQTTAsync.h>

COMM_MQTT_BEGIN_NAMESPACE

CommMQTTSettingsParser::CommMQTTSettingsParser(const QJsonObject &settings)
    : m_settings(settings)
{
}

std::string CommMQTTSettingsParser::protocol() const
{
    const QString protocol = m_settings.value("protocol").toString();
    return protocol.split(':').first().toStdString();
}

std::string CommMQTTSettingsParser::host() const
{
    return m_settings.value("host").toString().toStdString();
}

std::string CommMQTTSettingsParser::username() const
{
    return m_settings.value("username").toString().toStdString();
}

std::string CommMQTTSettingsParser::password() const
{
    return m_settings.value("password").toString().toStdString();
}

std::string CommMQTTSettingsParser::clientId() const
{
    return m_settings.value("clientId").toString().toStdString();
}

std::string CommMQTTSettingsParser::publishTopic() const
{
    return m_settings.value("publishTopic").toString().toStdString();
}

std::string CommMQTTSettingsParser::subscribeTopic() const
{
    return m_settings.value("subscribeTopic").toString().toStdString();
}

int CommMQTTSettingsParser::connectionTimeout() const
{
    return m_settings.value("connectionTimeout").toInt(10);
}

int CommMQTTSettingsParser::keepAlive() const
{
    return m_settings.value("keepAlive").toInt(60);
}

int CommMQTTSettingsParser::port() const
{
    return m_settings.value("port").toInt(-1);
}

int CommMQTTSettingsParser::version() const
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

int CommMQTTSettingsParser::publishQos() const
{
    return m_settings.value("publishQos").toInt(0);
}

int CommMQTTSettingsParser::subscribeQos() const
{
    return m_settings.value("subscribeQos").toInt(0);
}

bool CommMQTTSettingsParser::autoReconnect() const
{
    return m_settings.value("autoReconnect").toBool(false);
}

bool CommMQTTSettingsParser::cleanStart() const
{
    return m_settings.value("cleanStart").toBool(false);
}

bool CommMQTTSettingsParser::publish() const
{
    return m_settings.value("publish").toBool(false);
}

bool CommMQTTSettingsParser::subscribe() const
{
    return m_settings.value("subscribe").toBool(false);
}

COMM_MQTT_END_NAMESPACE
