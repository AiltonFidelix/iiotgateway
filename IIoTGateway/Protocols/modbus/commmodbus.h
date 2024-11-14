#ifndef COMMMODBUS_H
#define COMMMODBUS_H

#include "../comminterface.h"
#include "writeregistermodel.h"

#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusClient>

class CommModbus : public CommInterface
{
    Q_OBJECT
public:
    CommModbus() = default;
    ~CommModbus();

    void connectComm() = 0;
    void disconnectComm();
    bool isconnected();

public slots:
    void incoming(QByteArray data);

protected:
    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;

protected slots:
    void stateChanged(QModbusDevice::State state);
    void readRegisters();
    void writeRegisters();
    void readReady(QModbusReply *reply);

protected:
    QModbusClient *m_modbusDevice;

private:
    WriteRegisterModel m_writeModel;
};

#endif // COMMMODBUS_H
