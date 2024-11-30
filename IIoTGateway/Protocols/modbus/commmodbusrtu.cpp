#include "commmodbusrtu.h"
#include "modbusclientwrapper.h"
#include "commfactory.h"

#include <QTimer>

int CommModbusRTU::m_typeId = CommFactory::registerInterface<CommModbusRTU*>("MODBUS_RTU");

void
CommModbusRTU::connectComm()
{
    if (!m_modbusClient)
    {
        ModbusClientWrapper *wrapper = new ModbusClientWrapper();
        QModbusRtuSerialClient *client = new QModbusRtuSerialClient();
        wrapper->setModbusClient(client);

        m_modbusClient = static_cast<ModbusClientInterface*>(wrapper);
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

#warning // TODO: fix interface
        // connect(m_modbusClient, &QModbusClient::stateChanged, this, &CommModbusRTU::stateChanged);

        if (!m_modbusClient->connectDevice())
        {
            emit error(QString("Connect failed: %1").arg(m_modbusClient->errorString()).toUtf8());
        }
        else
        {
            QTimer::singleShot(1000, this, &CommModbusRTU::readRegisters);
            QTimer::singleShot(5000, this, &CommModbusRTU::writeRegisters);
        }
    }
}
