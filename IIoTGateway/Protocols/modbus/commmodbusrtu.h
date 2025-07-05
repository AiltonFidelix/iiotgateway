#ifndef COMMMODBUSRTU_H
#define COMMMODBUSRTU_H

#include <QModbusRtuSerialClient>
#include <QJsonObject>

#include "commmodbus_global.h"
#include "commmodbus.h"

COMM_MODBUS_BEGIN_NAMESPACE

class CommModbusRTU : public CommModbus
{
    Q_OBJECT

    static int m_typeId;

public:
    explicit Q_INVOKABLE CommModbusRTU(QJsonObject settings = QJsonObject{});
    ~CommModbusRTU() = default;

public slots:
    void connectComm() override;
};

COMM_MODBUS_END_NAMESPACE

#endif // COMMMODBUSRTU_H
