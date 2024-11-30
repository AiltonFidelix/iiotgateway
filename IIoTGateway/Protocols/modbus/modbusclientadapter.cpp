#include "modbusclientadapter.h"

ModbusClientAdapter::~ModbusClientAdapter()
{
    if (m_modbusClient)
    {
        delete m_modbusClient;
    }
}

bool
ModbusClientAdapter::connectDevice()
{
    return m_modbusClient->connectDevice();
}

void
ModbusClientAdapter::disconnectDevice()
{
    m_modbusClient->disconnectDevice();
}

void
ModbusClientAdapter::setConnectionParameter(int parameter, const QVariant &value)
{
    m_modbusClient->setConnectionParameter(static_cast<QModbusClient::ConnectionParameter>(parameter), value);
}

void
ModbusClientAdapter::setTimeout(int newTimeout)
{
    m_modbusClient->setTimeout(newTimeout);
}

void
ModbusClientAdapter::setNumberOfRetries(int number)
{
    m_modbusClient->setNumberOfRetries(number);
}

QModbusReply*
ModbusClientAdapter::sendReadRequest(const QModbusDataUnit &read, int serverAddress)
{
    return m_modbusClient->sendReadRequest(read, serverAddress);
}

QModbusReply*
ModbusClientAdapter::sendWriteRequest(const QModbusDataUnit &write, int serverAddress)
{
    return m_modbusClient->sendWriteRequest(write, serverAddress);
}

QModbusDevice::State
ModbusClientAdapter::state()
{
    return m_modbusClient->state();
}

QString
ModbusClientAdapter::errorString() const
{
    return m_modbusClient->errorString();
}

QModbusClient*
ModbusClientAdapter::modbusClient() const
{
    return m_modbusClient;
}

void
ModbusClientAdapter::setModbusClient(QModbusClient *newModbusClient)
{
    m_modbusClient = newModbusClient;

    connect(m_modbusClient, &QModbusClient::stateChanged, this, &ModbusClientAdapter::emitStateChanged);
}

void
ModbusClientAdapter::emitStateChanged(QModbusDevice::State state)
{
    emit stateChanged(state);
}
