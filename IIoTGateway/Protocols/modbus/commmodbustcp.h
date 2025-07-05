#ifndef COMMMODBUSTCP_H
#define COMMMODBUSTCP_H

#include <QModbusTcpClient>
#include <QJsonObject>

#include "commmodbus_global.h"
#include "commmodbus.h"

COMM_MODBUS_BEGIN_NAMESPACE

class CommModbusTCP : public CommModbus
{
    Q_OBJECT

    static int m_typeId;

public:
    explicit Q_INVOKABLE CommModbusTCP(QJsonObject settings = QJsonObject{});
    ~CommModbusTCP() = default;

public slots:
    void connectComm() override;
};

COMM_MODBUS_END_NAMESPACE

#endif // COMMMODBUSTCP_H
