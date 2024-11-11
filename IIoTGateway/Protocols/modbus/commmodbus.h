#ifndef COMMMODBUS_H
#define COMMMODBUS_H

#include "../comminterface.h"
#include "writeregistermodel.h"

#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusRtuSerialClient>

class CommModbus : public CommInterface
{
    Q_OBJECT
public:
    Q_INVOKABLE CommModbus();
    ~CommModbus();

    void connectComm();
    void disconnectComm();
    bool isconnected();

public slots:
    void incoming(QByteArray data);

private:
    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;

private slots:
    void stateChanged(QModbusDevice::State state);
    void readRegisters();
    void writeRegisters();
    void readReady(QModbusReply *reply);

private:
    static int m_typeId;

    QModbusRtuSerialClient m_modbusDevice;
    WriteRegisterModel m_writeModel;
};

#endif // COMMMODBUS_H
