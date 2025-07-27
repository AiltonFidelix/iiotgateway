#include "modbussettingsparser.h"

COMM_MODBUS_BEGIN_NAMESPACE

ModbusSettingsParser::ModbusSettingsParser(const QJsonObject &settings)
    : m_settings(settings)
{
}

QVariant ModbusSettingsParser::host() const
{
    return m_settings.value("host");
}

QVariant ModbusSettingsParser::port() const
{
    return m_settings.value("port");
}

QVariant ModbusSettingsParser::baudrate() const
{
    return m_settings.value("baudrate");
}

QVariant ModbusSettingsParser::parity() const
{
    return m_settings.value("parity");
}

int ModbusSettingsParser::databits() const
{
    return m_settings.value("dataBits").toInt(8);
}

int ModbusSettingsParser::stopbits() const
{
    return m_settings.value("stopBits").toInt(1);
}

int ModbusSettingsParser::retries() const
{
    return m_settings.value("retries").toInt(5);
}

int ModbusSettingsParser::timeout() const
{
    return m_settings.value("timeout").toInt(1000);
}

int ModbusSettingsParser::pollingTimeout() const
{
    return m_settings.value("pollingInterval").toInt(1000);
}

bool ModbusSettingsParser::pollingEnabled() const
{
    return m_settings.value("pollingEnabled").toBool();
}

COMM_MODBUS_END_NAMESPACE
