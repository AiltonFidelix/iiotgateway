#include "commmodbussettingsparser.hpp"

#include <QJsonArray>

COMM_MODBUS_BEGIN_NAMESPACE

CommModbusSettingsParser::CommModbusSettingsParser(QJsonObject settings)
    : _settings{std::move(settings)} {
}

QVariant CommModbusSettingsParser::host() const {
    return _settings.value(QStringLiteral("host")).toVariant();
}

QVariant CommModbusSettingsParser::port() const {
    return _settings.value(QStringLiteral("port")).toVariant();
}

QVariant CommModbusSettingsParser::parity() const {
    return _settings.value(QStringLiteral("parity"));
}

QVariant CommModbusSettingsParser::baudrate() const {
    return _settings.value(QStringLiteral("baudrate")).toVariant();
}

int CommModbusSettingsParser::databits() const {
    return _settings.value(QStringLiteral("dataBits")).toInt(8);
}

int CommModbusSettingsParser::stopbits() const {
    return _settings.value(QStringLiteral("stopBits")).toInt(1);
}

int CommModbusSettingsParser::retries() const {
    return _settings.value(QStringLiteral("retries")).toInt(5);
}

int CommModbusSettingsParser::timeout() const {
    return _settings.value(QStringLiteral("timeout")).toInt(1000);
}

int CommModbusSettingsParser::pollingInterval() const {
    return _settings.value(QStringLiteral("pollingInterval")).toInt(1000);
}

bool CommModbusSettingsParser::pollingEnabled() const {
    return _settings.value(QStringLiteral("pollingEnabled")).toBool();
}

Request CommModbusSettingsParser::requests() const {
    const QJsonValue value = _settings.value(QStringLiteral("requests"));

    if (!value.isArray()) {
        return Request();
    }

    QJsonObject devices{};
    devices.insert(QStringLiteral("devices"), value.toArray());

    const CommModbusRequestParser parser(QJsonDocument(devices).toJson(QJsonDocument::Compact));

    return parser.request();
}

QDebug &operator<<(QDebug &debug, const CommModbusSettingsParser &parser) {
    debug << parser._settings;

    return debug;
}

COMM_MODBUS_END_NAMESPACE
