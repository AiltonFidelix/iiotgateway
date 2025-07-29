#include "commmodbussettingsparser.h"

COMM_MODBUS_BEGIN_NAMESPACE

CommModbusSettingsParser::CommModbusSettingsParser(const QJsonObject &settings)
    : m_settings(settings)
{
}

QVariant CommModbusSettingsParser::host() const
{
    return m_settings.value("host");
}

QVariant CommModbusSettingsParser::port() const
{
    return m_settings.value("port");
}

QVariant CommModbusSettingsParser::baudrate() const
{
    return m_settings.value("baudrate");
}

QVariant CommModbusSettingsParser::parity() const
{
    return m_settings.value("parity");
}

int CommModbusSettingsParser::databits() const
{
    return m_settings.value("dataBits").toInt(8);
}

int CommModbusSettingsParser::stopbits() const
{
    return m_settings.value("stopBits").toInt(1);
}

int CommModbusSettingsParser::retries() const
{
    return m_settings.value("retries").toInt(5);
}

int CommModbusSettingsParser::timeout() const
{
    return m_settings.value("timeout").toInt(1000);
}

int CommModbusSettingsParser::pollingInterval() const
{
    return m_settings.value("pollingInterval").toInt(1000);
}

bool CommModbusSettingsParser::pollingEnabled() const
{
    return m_settings.value("pollingEnabled").toBool();
}

COMM_MODBUS_END_NAMESPACE
