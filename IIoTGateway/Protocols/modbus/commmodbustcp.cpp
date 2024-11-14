#include "commmodbustcp.h"
#include "commfactory.h"

#include <QTimer>

int CommModbusTCP::m_typeId = CommFactory::registerInterface<CommModbusTCP*>("MODBUS_TCP");

CommModbusTCP::CommModbusTCP()
{
    m_modbusDevice = new QModbusTcpClient();
}

void
CommModbusTCP::connectComm()
{
    if (m_modbusDevice->state() != QModbusDevice::ConnectedState)
    {
        QVariant port = qgetenv("MODBUS_TCP_PORT");
        QVariant host = qgetenv("MODBUS_TCP_HOST");

        bool ok = false;;

        int timeout = qEnvironmentVariableIntValue("MODBUS_TCP_TIMEOUT", &ok);

        timeout = !ok ? 1000 : timeout;

        int retries = qEnvironmentVariableIntValue("MODBUS_TCP_RETRIES", &ok);

        retries = !ok ? 3 : retries;

        m_modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
        m_modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, host);

        m_modbusDevice->setTimeout(timeout);
        m_modbusDevice->setNumberOfRetries(retries);

        connect(m_modbusDevice, &QModbusClient::stateChanged, this, &CommModbusTCP::stateChanged);

        if (!m_modbusDevice->connectDevice())
        {
            emit error(QString("Connect failed: %1").arg(m_modbusDevice->errorString()).toUtf8());
        }
        else
        {
            QTimer::singleShot(1000, this, &CommModbusTCP::readRegisters);
            QTimer::singleShot(500, this, &CommModbusTCP::writeRegisters);
        }
    }
}
