#include "commmodbusclientadapter.hpp"

COMM_MODBUS_BEGIN_NAMESPACE

CommModbusClientAdapter::CommModbusClientAdapter()
    : _modbusClient{nullptr} {
}

CommModbusClientAdapter::~CommModbusClientAdapter() {
    if (_modbusClient) {
        delete _modbusClient;
    }
}

bool CommModbusClientAdapter::connectDevice() const {
    return _modbusClient->connectDevice();
}

void CommModbusClientAdapter::disconnectDevice() {
    _modbusClient->disconnectDevice();
}

void CommModbusClientAdapter::setConnectionParameter(int parameter, const QVariant &value) {
    _modbusClient->setConnectionParameter(static_cast<QModbusClient::ConnectionParameter>(parameter), value);
}

void CommModbusClientAdapter::setTimeout(int newTimeout) {
    _modbusClient->setTimeout(newTimeout);
}

void CommModbusClientAdapter::setNumberOfRetries(int number) {
    _modbusClient->setNumberOfRetries(number);
}

QModbusReply *CommModbusClientAdapter::sendReadRequest(const QModbusDataUnit &read, int serverAddress) {
    return _modbusClient->sendReadRequest(read, serverAddress);
}

QModbusReply *CommModbusClientAdapter::sendWriteRequest(const QModbusDataUnit &write, int serverAddress) {
    return _modbusClient->sendWriteRequest(write, serverAddress);
}

QModbusDevice::State CommModbusClientAdapter::state() const {
    return _modbusClient->state();
}

QString CommModbusClientAdapter::errorString() const {
    return _modbusClient->errorString();
}

QModbusClient *CommModbusClientAdapter::modbusClient() const {
    return _modbusClient;
}

void CommModbusClientAdapter::setModbusClient(QModbusClient *newModbusClient) {
    _modbusClient = newModbusClient;

    connect(_modbusClient, &QModbusClient::stateChanged, this, &CommModbusClientAdapter::emitStateChanged);
}

void CommModbusClientAdapter::emitStateChanged(QModbusDevice::State state) {
    emit stateChanged(state);
}

COMM_MODBUS_END_NAMESPACE
