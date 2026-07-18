#ifndef COMMMODBUSTCP_H
#define COMMMODBUSTCP_H

#include <QJsonObject>
#include <QModbusTcpClient>

#include "commmodbus.hpp"
#include "commmodbus_global.hpp"

COMM_MODBUS_BEGIN_NAMESPACE

class CommModbusTCP final : public CommModbus {
    Q_OBJECT
public:
    Q_INVOKABLE explicit CommModbusTCP(QJsonObject settings = QJsonObject());

public slots:
    void connectComm() override;

private:
    static int _typeId;
};

COMM_MODBUS_END_NAMESPACE

#endif  // COMMMODBUSTCP_H
