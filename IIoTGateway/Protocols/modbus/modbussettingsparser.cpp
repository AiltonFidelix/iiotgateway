#include "modbussettingsparser.h"

COMM_MODBUS_BEGIN_NAMESPACE

ModbusSettingsParser::ModbusSettingsParser(QJsonObject settings)
    : m_settings(settings)
{
}

COMM_MODBUS_END_NAMESPACE
