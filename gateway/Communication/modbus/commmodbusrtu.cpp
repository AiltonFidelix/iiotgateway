#include "commmodbusrtu.hpp"

#include "commfactory.hpp"
#include "commmodbusclientadapter.hpp"

COMM_MODBUS_BEGIN_NAMESPACE

int CommModbusRTU::_typeId = comm::CommFactory::registerInterface<CommModbusRTU *>("MODBUS_RTU");

CommModbusRTU::CommModbusRTU(QJsonObject settings)
    : CommModbus{std::move(settings)} {
}

void CommModbusRTU::connectComm() {
    if (!_modbusClient) {
        auto adapter = new CommModbusClientAdapter();
        auto client = new QModbusRtuSerialClient();
        adapter->setModbusClient(client);

        _modbusClient = static_cast<CommModbusClientInterface *>(adapter);
    }

    if (_modbusClient->state() != QModbusDevice::ConnectedState) {
        if (_settingsParser.pollingEnabled()) {
            _polling = new QTimer();
            _polling->setInterval(_settingsParser.pollingInterval());

            connect(this, &CommModbusRTU::connected, this, &CommModbusRTU::initPolling);
        }

        _modbusClient->setConnectionParameter(QModbusDevice::SerialPortNameParameter, _settingsParser.port());
        _modbusClient->setConnectionParameter(QModbusDevice::SerialParityParameter, _settingsParser.parity());
        _modbusClient->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, _settingsParser.baudrate());
        _modbusClient->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, _settingsParser.databits());
        _modbusClient->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, _settingsParser.stopbits());

        _modbusClient->setTimeout(_settingsParser.timeout());
        _modbusClient->setNumberOfRetries(_settingsParser.retries());

        connect(_modbusClient, &CommModbusClientInterface::stateChanged, this, &CommModbusRTU::stateChanged);

        if (!_modbusClient->connectDevice()) {
            emit error(QString("Connection failed: %1").arg(_modbusClient->errorString()).toUtf8());
            emit connectionFailed();
        } else {
            emit connected();
        }
    }
}

COMM_MODBUS_END_NAMESPACE
