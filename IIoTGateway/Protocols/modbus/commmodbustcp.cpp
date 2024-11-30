#include "commmodbustcp.h"
#include "modbusclientwrapper.h"
#include "commfactory.h"

#include <QTimer>

int CommModbusTCP::m_typeId = CommFactory::registerInterface<CommModbusTCP*>("MODBUS_TCP");

void
CommModbusTCP::connectComm()
{
    if (!m_modbusClient)
    {
        ModbusClientWrapper *wrapper = new ModbusClientWrapper();
        QModbusTcpClient *client = new QModbusTcpClient();
        wrapper->setModbusClient(client);

        m_modbusClient = static_cast<ModbusClientInterface*>(wrapper);
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

        m_modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
        m_modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, host);

        m_modbusClient->setTimeout(timeout);
        m_modbusClient->setNumberOfRetries(retries);

#warning // TODO: fix interface
        // connect(m_modbusClient, &QModbusClient::stateChanged, this, &CommModbusTCP::stateChanged);

        if (!m_modbusClient->connectDevice())
        {
            emit error(QString("Connect failed: %1").arg(m_modbusClient->errorString()).toUtf8());
        }
        else
        {
            QTimer::singleShot(1000, this, &CommModbusTCP::readRegisters);
            QTimer::singleShot(500, this, &CommModbusTCP::writeRegisters);
        }
    }
}
