#ifndef MODBUSCLIENTINTERFACE_H
#define MODBUSCLIENTINTERFACE_H

#include <QModbusDevice>
#include <QModbusReply>

class ModbusClientInterface
{
public:
    virtual ~ModbusClientInterface() = default;

    virtual bool connectDevice() = 0;
    virtual void disconnectDevice() = 0;
    virtual void setConnectionParameter(int parameter, const QVariant &value) = 0;
    virtual void setTimeout(int newTimeout) = 0;
    virtual void setNumberOfRetries(int number) = 0;
    virtual QModbusReply *sendReadRequest(const QModbusDataUnit &read, int serverAddress) = 0;
    virtual QModbusReply *sendWriteRequest(const QModbusDataUnit &write, int serverAddress) = 0;
    virtual QModbusDevice::State state() = 0;
    virtual QString errorString() const = 0;
};

#endif // MODBUSCLIENTINTERFACE_H
