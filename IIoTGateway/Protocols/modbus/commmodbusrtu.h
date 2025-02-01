#ifndef COMMMODBUSRTU_H
#define COMMMODBUSRTU_H

#include "commmodbus.h"

#include <QModbusRtuSerialClient>

class CommModbusRTU : public CommModbus
{
    Q_OBJECT
public:
    Q_INVOKABLE CommModbusRTU() = default;
    ~CommModbusRTU() = default;

    void connectComm() override;

private:
    static int m_typeId;
};

#endif // COMMMODBUSRTU_H
