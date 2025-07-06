#ifndef COMMMODBUS_H
#define COMMMODBUS_H

#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusClient>
#include <QTimer>
#include <QJsonObject>

#include "comminterface.h"
#include "commmodbus_global.h"
#include "commmodbusclientinterface.h"
#include "commmodbusrequestparser.h"

COMM_MODBUS_BEGIN_NAMESPACE

using Registers = QMap<quint16, quint16>;

class CommModbus : public CommInterface
{
    Q_OBJECT

    bool ispolling();
    void readRegisters(const Request &request);
    void writeRegisters(const Request &request);

    Registers readReady(QModbusReply *reply);

    Request loadReadRequestSettings();
    QJsonArray registersToJsonArray(const Registers &registers);

public:
    explicit CommModbus(QJsonObject settings = QJsonObject());
    ~CommModbus();

    bool isconnected() override;

    CommModbusClientInterface *modbusClient();
    void setModbusClient(CommModbusClientInterface *client);

public slots:
    void disconnectComm() override;
    void incoming(QByteArray data) override;

protected slots:
    void stateChanged(QModbusDevice::State state);
    void initPolling();
    void pollingCallback();

protected:
    CommModbusClientInterface *m_modbusClient;
    QTimer *m_polling;
    Request m_readRequest;
};

COMM_MODBUS_END_NAMESPACE

#endif // COMMMODBUS_H
