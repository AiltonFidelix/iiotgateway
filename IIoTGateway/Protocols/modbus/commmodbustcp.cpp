#include "commmodbustcp.h"
#include "commmodbusclientadapter.h"
#include "commfactory.h"

COMM_MODBUS_BEGIN_NAMESPACE

int CommModbusTCP::m_typeId = comm::CommFactory::registerInterface<CommModbusTCP*>("MODBUS_TCP");

CommModbusTCP::CommModbusTCP(const QJsonObject &settings)
    : CommModbus(std::move(settings))
{
}

void CommModbusTCP::connectComm()
{
    if (!m_modbusClient)
    {
        auto adapter = new CommModbusClientAdapter();
        auto client = new QModbusTcpClient();
        adapter->setModbusClient(client);

        m_modbusClient = static_cast<CommModbusClientInterface*>(adapter);
    }

    if (m_modbusClient->state() != QModbusDevice::ConnectedState)
    {
        if (m_settingsParser.pollingEnabled())
        {
            m_polling = new QTimer();
            m_polling->setInterval(m_settingsParser.pollingInterval());

            connect(this, &CommModbusTCP::connected, this, &CommModbusTCP::initPolling);
        }

        m_modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, m_settingsParser.host());
        m_modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, m_settingsParser.port());

        m_modbusClient->setTimeout(m_settingsParser.timeout());
        m_modbusClient->setNumberOfRetries(m_settingsParser.retries());

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
