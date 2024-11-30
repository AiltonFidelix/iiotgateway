#include <gmock/gmock.h>
#include "modbus/modbusclientinterface.h"

class MockModbusClient : public ModbusClientInterface
{
public:
    MockModbusClient() = default;

    MOCK_METHOD(bool, connectDevice, (), (override));
    MOCK_METHOD(void, disconnectDevice, (), (override));
    MOCK_METHOD(void, setConnectionParameter, (int, const QVariant&), (override));
    MOCK_METHOD(void, setTimeout, (int), (override));
    MOCK_METHOD(void, setNumberOfRetries, (int), (override));
    MOCK_METHOD(QModbusReply*, sendReadRequest, (const QModbusDataUnit &, int), (override));
    MOCK_METHOD(QModbusReply*, sendWriteRequest, (const QModbusDataUnit &, int), (override));
    MOCK_METHOD(QModbusDevice::State, state, (), (override));
    MOCK_METHOD(QString, errorString, (), (const));
};
