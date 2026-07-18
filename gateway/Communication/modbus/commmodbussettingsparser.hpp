#ifndef COMMMODBUSSETTINGSPARSER_H
#define COMMMODBUSSETTINGSPARSER_H

#include <QJsonObject>

#include "commmodbus_global.hpp"
#include "commmodbusrequestparser.hpp"

COMM_MODBUS_BEGIN_NAMESPACE

class CommModbusSettingsParser {
public:
    explicit CommModbusSettingsParser(QJsonObject settings = QJsonObject());

    QVariant host() const;
    QVariant port() const;
    QVariant parity() const;
    QVariant baudrate() const;

    int databits() const;
    int stopbits() const;
    int retries() const;
    int timeout() const;
    int pollingInterval() const;

    bool pollingEnabled() const;

    Request requests() const;

    friend QDebug &operator<<(QDebug &debug, const CommModbusSettingsParser &parser);

private:
    QJsonObject _settings;
};

COMM_MODBUS_END_NAMESPACE

#endif  // COMMMODBUSSETTINGSPARSER_H
