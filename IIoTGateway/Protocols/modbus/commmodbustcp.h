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

public slots:
    void connectComm() override;

private:
    static int m_typeId;
};

#endif // COMMMODBUSTCP_H
