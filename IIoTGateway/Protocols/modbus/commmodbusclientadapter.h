#ifndef COMMMODBUSCLIENTADAPTER_H
#define COMMMODBUSCLIENTADAPTER_H

#include <QModbusClient>

#include "commmodbus_global.h"
#include "commmodbusclientinterface.h"

COMM_MODBUS_BEGIN_NAMESPACE

class CommModbusClientAdapter : public CommModbusClientInterface
{
    QModbusClient *m_modbusClient;

public:
    CommModbusClientAdapter();
    ~CommModbusClientAdapter();

    bool connectDevice() override;
    void disconnectDevice() override;
    void setConnectionParameter(int parameter, const QVariant &value) override;
    void setTimeout(int newTimeout) override;
    void setNumberOfRetries(int number) override;
    QModbusReply *sendReadRequest(const QModbusDataUnit &read, int serverAddress) override;
    QModbusReply *sendWriteRequest(const QModbusDataUnit &write, int serverAddress) override;
    QModbusDevice::State state() override;
    QString errorString() const override;

    QModbusClient *modbusClient() const;
    void setModbusClient(QModbusClient *newModbusClient);

private slots:
    void emitStateChanged(QModbusDevice::State state);
};

COMM_MODBUS_END_NAMESPACE

#endif // COMMMODBUSCLIENTADAPTER_H
