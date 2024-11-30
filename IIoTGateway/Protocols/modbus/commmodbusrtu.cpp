#include "commmodbusrtu.h"
#include "modbusclientadapter.h"
#include "commfactory.h"

#include <QTimer>

int CommModbusRTU::m_typeId = CommFactory::registerInterface<CommModbusRTU*>("MODBUS_RTU");

void
CommModbusRTU::connectComm()
{
    if (!m_modbusClient)
    {
        ModbusClientAdapter *adapter = new ModbusClientAdapter();
        QModbusRtuSerialClient *client = new QModbusRtuSerialClient();
        adapter->setModbusClient(client);

        m_modbusClient = static_cast<ModbusClientInterface*>(adapter);
    }

    if (m_modbusClient->state() != QModbusDevice::ConnectedState)
    {
        QVariant port = qgetenv("MODBUS_RTU_PORT");
        QVariant parity = qgetenv("MODBUS_RTU_PARITY");
        QVariant baudRate = qgetenv("MODBUS_RTU_BAUDRATE");
        QVariant dataBits = qgetenv("MODBUS_RTU_DATABITS");
        QVariant stopBits = qgetenv("MODBUS_RTU_STOPBITS");

        bool ok = false;;

        int timeout = qEnvironmentVariableIntValue("MODBUS_RTU_TIMEOUT", &ok);

        timeout = !ok ? 1000 : timeout;

        int retries = qEnvironmentVariableIntValue("MODBUS_RTU_RETRIES", &ok);

        retries = !ok ? 3 : retries;

        m_modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter, port);
        m_modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter, parity);
        m_modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baudRate);
        m_modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, dataBits);
        m_modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopBits);

        m_modbusClient->setTimeout(timeout);
        m_modbusClient->setNumberOfRetries(retries);

        connect(m_modbusClient, &ModbusClientInterface::stateChanged, this, &CommModbusRTU::stateChanged);

        if (!m_modbusClient->connectDevice())
        {
            emit error(QString("Connection failed: %1").arg(m_modbusClient->errorString()).toUtf8());
        }
        else
        {
            QTimer::singleShot(1000, this, &CommModbusRTU::readRegisters);
            QTimer::singleShot(5000, this, &CommModbusRTU::writeRegisters);
        }
    }
}
