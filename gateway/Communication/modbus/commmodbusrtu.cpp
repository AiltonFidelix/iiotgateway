#include "commmodbusrtu.h"
#include "commmodbusclientadapter.h"
#include "commfactory.h"

COMM_MODBUS_BEGIN_NAMESPACE

int CommModbusRTU::m_typeId = comm::CommFactory::registerInterface<CommModbusRTU*>("MODBUS_RTU");

CommModbusRTU::CommModbusRTU(QJsonObject settings)
    : CommModbus{std::move(settings)}
{
}

void CommModbusRTU::connectComm()
{
    if (!m_modbusClient)
    {
        auto adapter = new CommModbusClientAdapter();
        auto client = new QModbusRtuSerialClient();
        adapter->setModbusClient(client);

        m_modbusClient = static_cast<CommModbusClientInterface*>(adapter);
    }

    if (m_modbusClient->state() != QModbusDevice::ConnectedState)
    {
        if (m_settingsParser.pollingEnabled())
        {
            m_polling = new QTimer();
            m_polling->setInterval(m_settingsParser.pollingInterval());

            connect(this, &CommModbusRTU::connected, this, &CommModbusRTU::initPolling);
        }

        m_modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter, m_settingsParser.port());
        m_modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter, m_settingsParser.parity());
        m_modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, m_settingsParser.baudrate());
        m_modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, m_settingsParser.databits());
        m_modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, m_settingsParser.stopbits());

        m_modbusClient->setTimeout(m_settingsParser.timeout());
        m_modbusClient->setNumberOfRetries(m_settingsParser.retries());

        connect(m_modbusClient, &CommModbusClientInterface::stateChanged, this, &CommModbusRTU::stateChanged);

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
