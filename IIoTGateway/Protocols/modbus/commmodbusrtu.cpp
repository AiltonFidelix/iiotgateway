#include "commmodbusrtu.h"
#include "commfactory.h"

#include <QTimer>

int CommModbusRTU::m_typeId = CommFactory::registerInterface<CommModbusRTU*>("MODBUS_RTU");

void
CommModbusRTU::connectComm()
{
    if (!m_modbusDevice)
    {
        m_modbusDevice = new QModbusRtuSerialClient();
    }

    if (m_modbusDevice->state() != QModbusDevice::ConnectedState)
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

        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, port);
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, parity);
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, baudRate);
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, dataBits);
        m_modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, stopBits);

        m_modbusDevice->setTimeout(timeout);
        m_modbusDevice->setNumberOfRetries(retries);

        connect(m_modbusDevice, &QModbusClient::stateChanged, this, &CommModbusRTU::stateChanged);

        if (!m_modbusDevice->connectDevice())
        {
            emit error(QString("Connect failed: %1").arg(m_modbusDevice->errorString()).toUtf8());
        }
        else
        {
            QTimer::singleShot(1000, this, &CommModbusRTU::readRegisters);
            QTimer::singleShot(5000, this, &CommModbusRTU::writeRegisters);
        }
    }
}
