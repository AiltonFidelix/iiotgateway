#ifndef COMMMODBUSSETTINGSPARSER_H
#define COMMMODBUSSETTINGSPARSER_H

#include <QJsonObject>

#include "commmodbus_global.h"

COMM_MODBUS_BEGIN_NAMESPACE

class CommModbusSettingsParser
{
    QJsonObject m_settings;

public:
    explicit CommModbusSettingsParser(const QJsonObject &settings = QJsonObject());
    ~CommModbusSettingsParser() = default;

    QVariant host() const;
    QVariant port() const;
    QVariant parity() const;
    QString baudrate() const;

    int databits() const;
    int stopbits() const;
    int retries() const;
    int timeout() const;
    int pollingInterval() const;

    bool pollingEnabled() const;

    friend QDebug &operator<<(QDebug &debug, const CommModbusSettingsParser &parser);
};

COMM_MODBUS_END_NAMESPACE

#endif // COMMMODBUSSETTINGSPARSER_H
