#ifndef COMMINTERFACE_H
#define COMMINTERFACE_H

#include <QObject>

class CommInterface : public QObject
{
    Q_OBJECT
public:
    virtual ~CommInterface() = default;

    virtual bool isconnected() = 0;

signals:
    void connected();
    void disconnected();
    void connectionFailed();
    void outgoing(QByteArray data);
    void error(QByteArray error);

public slots:
    virtual void connectComm() = 0;
    virtual void disconnectComm() = 0;
    virtual void incoming(QByteArray data) = 0;
};

#endif // COMMINTERFACE_H
