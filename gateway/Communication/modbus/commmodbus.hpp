#ifndef COMMMODBUS_H
#define COMMMODBUS_H

#include "comminterface.hpp"
#include "commmodbus_global.hpp"

#include "commmodbusclientinterface.hpp"
#include "commmodbusrequestparser.hpp"
#include "commmodbussettingsparser.hpp"

#include <QJsonObject>
#include <QModbusClient>
#include <QModbusDataUnit>
#include <QTimer>

COMM_MODBUS_BEGIN_NAMESPACE

using Registers = QMap<quint16, quint16>;

class CommModbus : public CommInterface
{
    Q_OBJECT
public:
    explicit CommModbus(QJsonObject settings = QJsonObject());
    ~CommModbus();

    bool isconnected() const override;

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
    CommModbusSettingsParser m_settingsParser;

private:
    bool ispolling();

    void readRegisters(const Request &request);
    void writeRegisters(const Request &request);

    Registers readReady(QModbusReply *reply);
    QJsonArray registersToJsonArray(const Registers &registers);
};

COMM_MODBUS_END_NAMESPACE

#endif // COMMMODBUS_H
