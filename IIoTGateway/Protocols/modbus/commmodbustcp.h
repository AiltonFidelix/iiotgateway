#ifndef COMMMODBUSTCP_H
#define COMMMODBUSTCP_H

#include "commmodbus.h"

#include <QModbusTcpClient>

class CommModbusTCP : public CommModbus
{
    Q_OBJECT
public:
    Q_INVOKABLE CommModbusTCP() = default;
    ~CommModbusTCP() = default;

    void connectComm();

private:
    static int m_typeId;
};

#endif // COMMMODBUSTCP_H
