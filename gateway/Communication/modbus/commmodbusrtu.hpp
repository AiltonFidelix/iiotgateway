#ifndef COMMMODBUSRTU_H
#define COMMMODBUSRTU_H

#include <QJsonObject>
#include <QModbusRtuSerialClient>

#include "commmodbus.hpp"
#include "commmodbus_global.hpp"

COMM_MODBUS_BEGIN_NAMESPACE

class CommModbusRTU final : public CommModbus {
    Q_OBJECT
public:
    Q_INVOKABLE explicit CommModbusRTU(QJsonObject settings = QJsonObject());

public slots:
    void connectComm() override;

private:
    static int _typeId;
};

COMM_MODBUS_END_NAMESPACE

#endif  // COMMMODBUSRTU_H
