#include "commmodbussettingsparser.h"

#include <QJsonArray>

COMM_MODBUS_BEGIN_NAMESPACE

CommModbusSettingsParser::CommModbusSettingsParser(QJsonObject settings)
    : m_settings{std::move(settings)}
{
}

QVariant CommModbusSettingsParser::host() const
{
    return m_settings.value(QStringLiteral("host")).toVariant();
}

QVariant CommModbusSettingsParser::port() const
{
    return m_settings.value(QStringLiteral("port")).toVariant();
}

QVariant CommModbusSettingsParser::parity() const
{
    return m_settings.value(QStringLiteral("parity"));
}

QVariant CommModbusSettingsParser::baudrate() const
{
    return m_settings.value(QStringLiteral("baudrate")).toVariant();
}

int CommModbusSettingsParser::databits() const
{
    return m_settings.value(QStringLiteral("dataBits")).toInt(8);
}

int CommModbusSettingsParser::stopbits() const
{
    return m_settings.value(QStringLiteral("stopBits")).toInt(1);
}

int CommModbusSettingsParser::retries() const
{
    return m_settings.value(QStringLiteral("retries")).toInt(5);
}

int CommModbusSettingsParser::timeout() const
{
    return m_settings.value(QStringLiteral("timeout")).toInt(1000);
}

int CommModbusSettingsParser::pollingInterval() const
{
    return m_settings.value(QStringLiteral("pollingInterval")).toInt(1000);
}

bool CommModbusSettingsParser::pollingEnabled() const
{
    return m_settings.value(QStringLiteral("pollingEnabled")).toBool();
}

Request CommModbusSettingsParser::requests() const
{
    const QJsonValue value = m_settings.value(QStringLiteral("requests"));

    if (!value.isArray())
    {
        return Request();
    }

    QJsonObject devices{};
    devices.insert(QStringLiteral("devices"), value.toArray());

    CommModbusRequestParser parser(QJsonDocument(devices).toJson(QJsonDocument::Compact));

    return parser.request();
}

QDebug &operator<<(QDebug &debug, const CommModbusSettingsParser &parser)
{
    debug << parser.m_settings;

    return debug;
}

COMM_MODBUS_END_NAMESPACE
