#include "commmodbustcp.hpp"

#include "commfactory.hpp"
#include "commmodbusclientadapter.hpp"

COMM_MODBUS_BEGIN_NAMESPACE

int CommModbusTCP::_typeId = comm::CommFactory::registerInterface<CommModbusTCP *>("MODBUS_TCP");

CommModbusTCP::CommModbusTCP(QJsonObject settings)
    : CommModbus{std::move(settings)} {
}

void CommModbusTCP::connectComm() {
    if (!_modbusClient) {
        auto adapter = new CommModbusClientAdapter();
        auto client = new QModbusTcpClient();
        adapter->setModbusClient(client);

        _modbusClient = static_cast<CommModbusClientInterface *>(adapter);
    }

    if (_modbusClient->state() != QModbusDevice::ConnectedState) {
        if (_settingsParser.pollingEnabled()) {
            _polling = new QTimer();
            _polling->setInterval(_settingsParser.pollingInterval());

            connect(this, &CommModbusTCP::connected, this, &CommModbusTCP::initPolling);
        }

        _modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, _settingsParser.host());
        _modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, _settingsParser.port());

        _modbusClient->setTimeout(_settingsParser.timeout());
        _modbusClient->setNumberOfRetries(_settingsParser.retries());

        connect(_modbusClient, &CommModbusClientInterface::stateChanged, this, &CommModbusTCP::stateChanged);

        if (!_modbusClient->connectDevice()) {
            emit error(QString("Connection failed: %1").arg(_modbusClient->errorString()).toUtf8());
            emit connectionFailed();
        } else {
            emit connected();
        }
    }
}

COMM_MODBUS_END_NAMESPACE
