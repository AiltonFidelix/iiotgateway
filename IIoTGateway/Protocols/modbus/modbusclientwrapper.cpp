#include "modbusclientwrapper.h"

ModbusClientWrapper::~ModbusClientWrapper()
{
    if (m_modbusClient)
    {
        delete m_modbusClient;
    }
}

bool
ModbusClientWrapper::connectDevice()
{
    return m_modbusClient->connectDevice();
}

void
ModbusClientWrapper::disconnectDevice()
{
    m_modbusClient->disconnectDevice();
}

void
ModbusClientWrapper::setConnectionParameter(int parameter, const QVariant &value)
{
    m_modbusClient->setConnectionParameter(static_cast<QModbusClient::ConnectionParameter>(parameter), value);
}

void
ModbusClientWrapper::setTimeout(int newTimeout)
{
    m_modbusClient->setTimeout(newTimeout);
}

void
ModbusClientWrapper::setNumberOfRetries(int number)
{
    m_modbusClient->setNumberOfRetries(number);
}

QModbusReply*
ModbusClientWrapper::sendReadRequest(const QModbusDataUnit &read, int serverAddress)
{
    return m_modbusClient->sendReadRequest(read, serverAddress);
}

QModbusReply*
ModbusClientWrapper::sendWriteRequest(const QModbusDataUnit &write, int serverAddress)
{
    return m_modbusClient->sendWriteRequest(write, serverAddress);
}

QModbusDevice::State
ModbusClientWrapper::state()
{
    return m_modbusClient->state();
}

QString
ModbusClientWrapper::errorString() const
{
    return m_modbusClient->errorString();
}

QModbusClient*
ModbusClientWrapper::modbusClient() const
{
    return m_modbusClient;
}

void
ModbusClientWrapper::setModbusClient(QModbusClient *newModbusClient)
{
    m_modbusClient = newModbusClient;
}
