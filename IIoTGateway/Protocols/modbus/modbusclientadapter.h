#ifndef MODBUSCLIENTADAPTER_H
#define MODBUSCLIENTADAPTER_H

#include <QModbusClient>
#include "modbusclientinterface.h"

class ModbusClientAdapter : public ModbusClientInterface
{
public:
    ModbusClientAdapter() = default;
    ~ModbusClientAdapter();

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

private slots:
    void emitStateChanged(QModbusDevice::State state);

private:
    QModbusClient *m_modbusClient;
};

#endif // MODBUSCLIENTADAPTER_H
