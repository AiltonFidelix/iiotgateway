#ifndef COMMMQTT_H
#define COMMMQTT_H

#include "../comminterface.h"

class CommMQTT : public CommInterface
{
    Q_OBJECT
public:
    Q_INVOKABLE CommMQTT() = default;
    ~CommMQTT() = default;

    bool isconnected() override;

public slots:
    void connectComm() override;
    void disconnectComm() override;
    void incoming(QByteArray data) override;

private:
    static int m_typeId;
};

#endif // COMMMQTT_H
