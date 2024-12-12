#ifndef COMMMODBUS_H
#define COMMMODBUS_H

#include "../comminterface.h"
#include "modbusclientinterface.h"
#include "modbusjsonparser.h"

#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusClient>
#include <QTimer>
#include <QDebug>

class CommModbus : public CommInterface
{
    Q_OBJECT
public:
    CommModbus();
    ~CommModbus();

    void connectComm() = 0;
    void disconnectComm();
    bool isconnected();

    ModbusClientInterface *modbusClient();
    void setModbusClient(ModbusClientInterface *client);

public slots:
    void incoming(QByteArray data);

protected slots:
    void stateChanged(QModbusDevice::State state);
    void readRegisters();
    void writeRegisters();
    void readReady(QModbusReply *reply);
    void initPolling();

private:
    ModbusJsonParser::Request loadReadRequestSettings();

protected:
    ModbusClientInterface *m_modbusClient;
    QTimer *m_polling;
    ModbusJsonParser::Request m_readRequest;
};

#endif // COMMMODBUS_H
