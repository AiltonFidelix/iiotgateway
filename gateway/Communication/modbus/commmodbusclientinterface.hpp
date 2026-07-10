#ifndef COMMMODBUSCLIENTINTERFACE_H
#define COMMMODBUSCLIENTINTERFACE_H

#include <QModbusDevice>
#include <QModbusReply>
#include <QObject>

#include "commmodbus_global.hpp"

COMM_MODBUS_BEGIN_NAMESPACE

class CommModbusClientInterface : public QObject {
    Q_OBJECT
public:
    virtual ~CommModbusClientInterface() = default;

    virtual bool connectDevice() const = 0;
    virtual void disconnectDevice() = 0;
    virtual void setConnectionParameter(int parameter, const QVariant &value) = 0;
    virtual void setTimeout(int newTimeout) = 0;
    virtual void setNumberOfRetries(int number) = 0;
    virtual QModbusReply *sendReadRequest(const QModbusDataUnit &read, int serverAddress) = 0;
    virtual QModbusReply *sendWriteRequest(const QModbusDataUnit &write, int serverAddress) = 0;
    virtual QModbusDevice::State state() const = 0;
    virtual QString errorString() const = 0;

signals:
    void stateChanged(QModbusDevice::State state);
};

COMM_MODBUS_END_NAMESPACE

#endif  // COMMMODBUSCLIENTINTERFACE_H
