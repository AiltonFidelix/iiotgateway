#include "commmodbusclientadapter.h"

COMM_MODBUS_BEGIN_NAMESPACE

CommModbusClientAdapter::CommModbusClientAdapter()
    : m_modbusClient{nullptr}
{
}

CommModbusClientAdapter::~CommModbusClientAdapter()
{
    if (m_modbusClient)
    {
        delete m_modbusClient;
    }
}

bool CommModbusClientAdapter::connectDevice() const
{
    return m_modbusClient->connectDevice();
}

void CommModbusClientAdapter::disconnectDevice()
{
    m_modbusClient->disconnectDevice();
}

void CommModbusClientAdapter::setConnectionParameter(int parameter, const QVariant &value)
{
    m_modbusClient->setConnectionParameter(static_cast<QModbusClient::ConnectionParameter>(parameter), value);
}

void CommModbusClientAdapter::setTimeout(int newTimeout)
{
    m_modbusClient->setTimeout(newTimeout);
}

void CommModbusClientAdapter::setNumberOfRetries(int number)
{
    m_modbusClient->setNumberOfRetries(number);
}

QModbusReply *CommModbusClientAdapter::sendReadRequest(const QModbusDataUnit &read, int serverAddress)
{
    return m_modbusClient->sendReadRequest(read, serverAddress);
}

QModbusReply *CommModbusClientAdapter::sendWriteRequest(const QModbusDataUnit &write, int serverAddress)
{
    return m_modbusClient->sendWriteRequest(write, serverAddress);
}

QModbusDevice::State CommModbusClientAdapter::state() const
{
    return m_modbusClient->state();
}

QString CommModbusClientAdapter::errorString() const
{
    return m_modbusClient->errorString();
}

QModbusClient *CommModbusClientAdapter::modbusClient() const
{
    return m_modbusClient;
}

void CommModbusClientAdapter::setModbusClient(QModbusClient *newModbusClient)
{
    m_modbusClient = newModbusClient;

    connect(m_modbusClient, &QModbusClient::stateChanged, this, &CommModbusClientAdapter::emitStateChanged);
}

void CommModbusClientAdapter::emitStateChanged(QModbusDevice::State state)
{
    emit stateChanged(state);
}

COMM_MODBUS_END_NAMESPACE
