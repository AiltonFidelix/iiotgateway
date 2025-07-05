#include "commmodbustcp.h"
#include "commmodbusclientadapter.h"
#include "commfactory.h"

#include <QVariant>

COMM_MODBUS_BEGIN_NAMESPACE

int CommModbusTCP::m_typeId = comm::CommFactory::registerInterface<CommModbusTCP*>("MODBUS_TCP");

CommModbusTCP::CommModbusTCP(QJsonObject settings)
    : CommModbus{settings}
{
}

void CommModbusTCP::connectComm()
{
    if (!m_modbusClient)
    {
        auto adapter = new CommModbusClientAdapter{};
        auto client = new QModbusTcpClient{};
        adapter->setModbusClient(client);

        m_modbusClient = static_cast<CommModbusClientInterface*>(adapter);
    }

    if (m_modbusClient->state() != QModbusDevice::ConnectedState)
    {
#warning // TODO: change to settings on JSON
        QVariant port = qgetenv("MODBUS_TCP_PORT");
        QVariant host = qgetenv("MODBUS_TCP_HOST");

        bool ok = false;;

        int timeout = qEnvironmentVariableIntValue("MODBUS_TCP_TIMEOUT", &ok);

        timeout = !ok ? 1000 : timeout;

        int retries = qEnvironmentVariableIntValue("MODBUS_TCP_RETRIES", &ok);

        retries = !ok ? 3 : retries;

        QVariant polling = qgetenv("MODBUS_TCP_POLLING");

        int interval = qEnvironmentVariableIntValue("MODBUS_TCP_INTERVAL", &ok);

        interval = !ok ? 1000 : interval;

        if (polling.toString().toLower() == "enabled")
        {
            m_polling = new QTimer();
            m_polling->setInterval(interval);

            connect(this, &CommModbusTCP::connected, this, &CommModbusTCP::initPolling);
        }

        m_modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
        m_modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, host);

        m_modbusClient->setTimeout(timeout);
        m_modbusClient->setNumberOfRetries(retries);

        connect(m_modbusClient, &CommModbusClientInterface::stateChanged, this, &CommModbusTCP::stateChanged);

        if (!m_modbusClient->connectDevice())
        {
            emit error(QString("Connection failed: %1").arg(m_modbusClient->errorString()).toUtf8());
            emit connectionFailed();
        }
        else
        {
            emit connected();
        }
    }
}

COMM_MODBUS_END_NAMESPACE
