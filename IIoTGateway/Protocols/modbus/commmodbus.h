#ifndef COMMMODBUS_H
#define COMMMODBUS_H

#include "../comminterface.h"
#include "modbusclientinterface.h"
#include "writeregistermodel.h"

#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusClient>

class CommModbus : public CommInterface
{
    Q_OBJECT
public:
    using ReadItems = QHash<quint8, QList<quint16>>;
    using WriteItems = QHash<quint8, QHash<quint16, quint16>>;

    CommModbus() = default;
    ~CommModbus();

    void connectComm() = 0;
    void disconnectComm();
    bool isconnected();

    void setModbusClient(ModbusClientInterface *client);

public slots:
    void incoming(QByteArray data);

protected:
    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;

protected slots:
    void stateChanged(QModbusDevice::State state);
    void readRegisters();
    void writeRegisters();
    void readReady();

protected:
    ModbusClientInterface *m_modbusClient;
    ReadItems m_readItems;

private:
    WriteRegisterModel m_writeModel;
};

#endif // COMMMODBUS_H
