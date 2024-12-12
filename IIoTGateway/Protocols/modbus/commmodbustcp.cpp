#include "commmodbustcp.h"
#include "modbusclientadapter.h"
#include "commfactory.h"

#include <QVariant>

int CommModbusTCP::m_typeId = CommFactory::registerInterface<CommModbusTCP*>("MODBUS_TCP");

void
CommModbusTCP::connectComm()
{
    if (!m_modbusClient)
    {
        ModbusClientAdapter *adapter = new ModbusClientAdapter();
        QModbusTcpClient *client = new QModbusTcpClient();
        adapter->setModbusClient(client);

        m_modbusClient = static_cast<ModbusClientInterface*>(adapter);
    }

    if (m_modbusClient->state() != QModbusDevice::ConnectedState)
    {
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

        connect(m_modbusClient, &ModbusClientInterface::stateChanged, this, &CommModbusTCP::stateChanged);

        if (!m_modbusClient->connectDevice())
        {
            emit error(QString("Connection failed: %1").arg(m_modbusClient->errorString()).toUtf8());
        }
        else
        {
            emit connected();
        }
    }
}
