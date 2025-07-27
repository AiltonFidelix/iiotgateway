#ifndef MODBUSSETTINGSPARSER_H
#define MODBUSSETTINGSPARSER_H

#include <QJsonObject>

#include "commmodbus_global.h"

COMM_MODBUS_BEGIN_NAMESPACE

class ModbusSettingsParser
{
    QJsonObject m_settings;

public:
    explicit ModbusSettingsParser(QJsonObject settings = QJsonObject());
    ~ModbusSettingsParser() = default;
};

COMM_MODBUS_END_NAMESPACE

#endif // MODBUSSETTINGSPARSER_H
