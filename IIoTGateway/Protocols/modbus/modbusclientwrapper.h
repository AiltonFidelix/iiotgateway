#ifndef MODBUSCLIENTWRAPPER_H
#define MODBUSCLIENTWRAPPER_H

#include <QModbusClient>
#include "modbusclientinterface.h"

class ModbusClientWrapper : public ModbusClientInterface
{
public:
    ModbusClientWrapper() = default;
    ~ModbusClientWrapper();

    bool connectDevice();
    void disconnectDevice();
    void setConnectionParameter(int parameter, const QVariant &value);
    void setTimeout(int newTimeout);
    void setNumberOfRetries(int number);
    QModbusReply *sendReadRequest(const QModbusDataUnit &read, int serverAddress);
    QModbusReply *sendWriteRequest(const QModbusDataUnit &write, int serverAddress);
    QModbusDevice::State state();
    QString errorString() const;

    QModbusClient *modbusClient() const;
    void setModbusClient(QModbusClient *newModbusClient);

private:
    QModbusClient *m_modbusClient;
};

#endif // MODBUSCLIENTWRAPPER_H
