#ifndef COMMMODBUS_H
#define COMMMODBUS_H

#include "../comminterface.h"
#include "modbusclientinterface.h"
#include "modbusjsonparser.h"

#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusClient>
#include <QTimer>

class CommModbus : public CommInterface
{
    using Registers = QMap<quint16, quint16>;

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
    void initPolling();
    void pollingCallback();

private:
    bool ispolling();
    void readRegisters(const ModbusJsonParser::Request &request);
    void writeRegisters(const ModbusJsonParser::Request &request);

    Registers readReady(QModbusReply *reply);

    ModbusJsonParser::Request loadReadRequestSettings();
    QJsonArray registersToJsonArray(const Registers &registers);

protected:
    ModbusClientInterface *m_modbusClient;
    QTimer *m_polling;
    ModbusJsonParser::Request m_readRequest;
};

#endif // COMMMODBUS_H
