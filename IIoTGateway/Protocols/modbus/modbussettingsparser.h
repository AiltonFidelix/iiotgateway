#ifndef MODBUSSETTINGSPARSER_H
#define MODBUSSETTINGSPARSER_H

#include <QJsonObject>

#include "commmodbus_global.h"

COMM_MODBUS_BEGIN_NAMESPACE

class ModbusSettingsParser
{
    QJsonObject m_settings;

public:
    explicit ModbusSettingsParser(const QJsonObject &settings = QJsonObject());
    ~ModbusSettingsParser() = default;

    QVariant host() const;
    QVariant port() const;
    QVariant baudrate() const;
    QVariant parity() const;

    int databits() const;
    int stopbits() const;
    int retries() const;
    int timeout() const;
    int pollingTimeout() const;

    bool pollingEnabled() const;
};

COMM_MODBUS_END_NAMESPACE

#endif // MODBUSSETTINGSPARSER_H
